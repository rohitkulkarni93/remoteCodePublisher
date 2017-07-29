#pragma once
//////////////////////////////////////////////////////////////////////
//  TypeAnalysis.h - Builds the type table							//
//  ver 1.0															//
//																	//
//  Lanaguage:     Visual C++										//
//  Platform:      Lenovo Z580, Windows 10 Pro						//
//  Application:   Dependency analyzer, OOD Project 2				//
//	Source:		   Dr. Jim Fawcett, Syracuse University				//
//  Author:        Rohit Kulkarni, Syracuse University				//
//                 rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////
/*
Module Operations:
------------------
This package is used to build the type table. The type table consists
of all the types in the files required for dependency analysis.
It uses the TypeTable to store the type information which it extracts
	
Required files:
---------------
TypeAnalysis.h

Maintenance History:
====================
ver 1.0 
	First release.
*/

#include "DepAnalysisEngine.h"
#include "../Parser/ActionsAndRules.h"
#include <iostream>
#include <functional>
#include "../Core.DBEngine/NoSQLDB.h"
#include "../Core.Datastructures/DatabaseStructure.h"
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../FileSystem/FileSystem.h"
#include "../Core.DBEngine/IQueryChainer.h"
#include "TypeTable.h"
#include <algorithm>
#include "../AbstractSyntaxTree/AbstrSynTree.h"

#pragma warning (disable : 4101)  // disable warning re unused variable x, below

using Helper = Utilities::StringHelper;
using namespace std;

namespace CodeAnalysis
{
	class TypeAnal
	{
	public:
		using SPtr = std::shared_ptr<ASTNode*>;

		TypeAnal();
		void doTypeAnal(string path);
		TypeTable exportTypeTable() const;
	private:
		void DFS(ASTNode* pNode);
		void processNode(ASTNode *);
		bool processUsingDecl(DeclarationNode node, string & value);
		string getFullyQualifiedName(string token, string type);
		void addToTypeTable(string typeName, string typeInfo, string package, string path, string FQN);
		void captureContext(ASTNode * node, bool capture);

		AbstrSynTree& ASTref_;
		ScopeStack<ASTNode*> scopeStack_;
		Scanner::Toker& toker_;

		TypeTable * table;
		stack<ASTNode *> * contextStack = new stack<ASTNode *>();
		int counter = 0;
		string path = "";
	};

	inline TypeAnal::TypeAnal() :
		ASTref_(Repository::getInstance()->AST()),
		scopeStack_(Repository::getInstance()->scopeStack()),
		toker_(*(Repository::getInstance()->Toker()))
	{
		table = new TypeTable();
		std::function<void()> test = [] { int x; };		// This is here to test detection of lambdas.
	}													// It doesn't do anything useful for dep anal.

	inline TypeTable TypeAnal::exportTypeTable() const
	{
		return *this->table;
	}

	inline void TypeAnal::processNode(ASTNode * pNode)
	{
		if (pNode->type_ == "class" || pNode->type_ == "struct") {
			this->addToTypeTable(pNode->name_, pNode->type_, pNode->package_,
				pNode->path_, getFullyQualifiedName(pNode->name_, pNode->type_));
		}
		if (pNode->type_ == "namespace") {
			for (ASTNode * child : pNode->children_) {
				if (child->type_ == "function") {
					this->addToTypeTable(child->name_, child->type_, child->package_,
						child->path_, getFullyQualifiedName(child->name_, child->type_));
				}
			}
		}
		if (pNode->decl_.size() > 0) {
			//	using declarations.
			for (auto iter = pNode->decl_.begin(); iter != pNode->decl_.end(); ++iter) {
				if (iter->declType_ == DeclType::usingDecl) {
					string value = "";
					bool isUsingStatement = processUsingDecl(*iter, value);
					if (isUsingStatement) {
						this->addToTypeTable(value, "usingDecl", iter->package_, pNode->path_, "");
					}
				}
			}
		}
	}

	//	Process using declarations. These are not directly added
	//	to the AbstractSyntaxTree by the CodeAnalyzer while parsing file.
	//	this function extracts using declarations, and adds it to the typetable.
	inline bool TypeAnal::processUsingDecl(DeclarationNode node, string & value) {
		vector<string> tokenVector;
		int tokens = node.pTc->length(), i = 0;
		while (i != tokens) {
			tokenVector.push_back(node.pTc->operator[](i++));
		}
		if (tokenVector[0] == "using" && tokenVector[2] == "=") {
			value = tokenVector[1];
			return true;
		}
		return false;
	}

	//	This function returns the fully qualified name of 
	inline string TypeAnal::getFullyQualifiedName(string token, string type) {
		string fqn;// = "::";
		stack<ASTNode *> tempStack;
		if (!contextStack->empty()) {
			//	Remove current context
			if (contextStack->top()->name_ == token)
			{
				tempStack.push(contextStack->top());
				contextStack->pop();
			}
		}
		while (!contextStack->empty() && contextStack->top()->type_ == "namespace") {
			tempStack.push(contextStack->top());
			contextStack->pop();
		}
		contextStack->push(tempStack.top());
		tempStack.pop();
		while (!tempStack.empty()) {
			//	Restore context stack
			auto node = tempStack.top();
			contextStack->push(node);
			fqn += "::" + node->name_;
			tempStack.pop();
		}
		return fqn;
	}

	inline void TypeAnal::addToTypeTable(string typeName,
		string typeInfo,
		string package,
		string path,
		string fullyQualifiedName) {

		Type typeTableEntry;
		typeTableEntry.typeName = typeName;
		typeTableEntry.typeInfo = typeInfo;
		typeTableEntry.packageName = package;
		typeTableEntry.pathName = path;
		typeTableEntry.fullyQualifiedName = fullyQualifiedName;
		this->table->addTypeToTable(typeTableEntry);
	}

	//	Perform DFS on the AST and process each node.
	inline void TypeAnal::DFS(ASTNode* pNode)
	{
		captureContext(pNode, true);
		static std::string path = "";
		if (pNode->path_ != path)
		{
			std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_ << endl;
			path = pNode->path_;
		}
		processNode(pNode);
		for (auto pChild : pNode->children_)
			DFS(pChild);
		captureContext(pNode, false);
	}

	//	Capture and release the current scope or context.
	inline void TypeAnal::captureContext(ASTNode * node, bool capture) {
		if (node->type_ == "namespace" || node->type_ == "class" || node->type_ == "struct") {
			if (capture)
				contextStack->push(node);
			else if (contextStack->top()->name_ == node->name_)
				contextStack->pop();
		}
	}

	//	This function performs the type analysis.
	inline void TypeAnal::doTypeAnal(string path)
	{
		if (path.length() > 0) {
			this->path = path;
		}
		std::cout << "\n  starting type analysis:\n";
		std::cout << "\n  scanning AST and displaying important things:";
		std::cout << "\n -----------------------------------------------\n\n";
		ASTNode* pRoot = ASTref_.root();
		DFS(pRoot);
		//	Scan files using tokenizer and create a dependency analysis table.
		this->table->commit();
		this->table->printTypeTable();
	}
}