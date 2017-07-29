//////////////////////////////////////////////////////////////////////
//  DepAnalysisEngine.cpp - Dependency Analyzer engine				//
//  ver 1.0															//
//																	//
//  Lanaguage:     Visual C++										//
//  Platform:      Lenovo Z580, Windows 10 Pro						//
//  Application:   Dependency analyzer, OOD Project 2				//
//  Author:        Rohit Kulkarni, Syracuse University				//
//                 rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////


#include <queue>
#include <string>
#include <vector>
#include <algorithm>
#include "../FileSystem/FileSystem.h"
#include "TypeTable.h"
#include "./DepAnalysisEngine.h"
#include <fstream>
#include "../Core.Datastructures/StrHelper.h"

using namespace std;

void DependencyAnalysisEngine::startDependencyAnalysis(const char * path)
{
	cout << "Starting dependency analysis: " << path << endl;
	cout << "---------------------------------------------------------------------" << endl;
	if (!path|| !FileSystem::Directory::exists(path))
		return;
	
	processDirectory(path);
	cout << "Depencency Analysis complete..." << endl;
}

void DependencyAnalysisEngine::exportAnalysisResultToFile(string exportFileName, string path)
{
	using SPtr = shared_ptr<AbstractXmlElement>;
	if (this->dependencies->size() > 0) {

		XmlProcessing::XmlDocument document;
		exportFileName = exportFileName == "" ? "Export.xml" : exportFileName;
		exportFileName = "./" + exportFileName;

		SPtr pRoot = makeTaggedElement("DependencyAnalysis");
		document.docElement() = pRoot;

		for (auto iter = dependencies->begin(); iter != dependencies->end(); ++iter) {

			SPtr dependencyTag = makeTaggedElement("Dep");
			SPtr depFileTag = makeTaggedElement("File");
			SPtr depFileText = makeTextElement(iter->first);
			SPtr indepFiles = makeTaggedElement("DependsOn");

			pRoot->addChild(dependencyTag);
			dependencyTag->addChild(depFileTag);
			depFileTag->addChild(depFileText);
			dependencyTag->addChild(indepFiles);

			for (auto str : iter->second) {

				SPtr indepFileTag = makeTaggedElement("File");
				SPtr indepFileText = makeTextElement(str);
				indepFileTag->addChild(indepFileText);
				indepFiles->addChild(indepFileTag);
			}
		}
		
		FileSystem::Directory::setCurrentDirectory(path);
		ofstream out(exportFileName, fstream::out | fstream::trunc);
		out << document.docElement()->toString();
		out.flush();
		out.close();
	}
}

unordered_map<string, vector<string>> DependencyAnalysisEngine::readAnalysisResultFromFile(string fileName)
{
	using SPtr = shared_ptr<AbstractXmlElement>;
	unordered_map<string, vector<string>> dependencies;
	if (FileSystem::File::exists(fileName))
	{
		XmlDocument doc(fileName, XmlDocument::file);
		std::vector<SPtr> desc = doc.element("DependencyAnalysis").select();
		for (auto dependency : desc[0]->children()) {
			string fileName = trim(dependency->children()[0]->children()[0]->value());
			vector<string> deps;
			for (auto depFile : dependency->children()[1]->children())
				deps.push_back(trim(depFile->children()[0]->value()));
			dependencies[fileName] = deps;
		}
	}
	return dependencies;
}

void DependencyAnalysisEngine::processDirectory(string path)
{
	using namespace std;
	try {
		FileSystem::Directory::setCurrentDirectory(path);
		processFiles(FileSystem::Directory::getFiles(".", "*.h"));
		processFiles(FileSystem::Directory::getFiles(".", "*.cpp"));
		auto directories = FileSystem::Directory::getDirectories();
		for each (string dir in directories)
		{
			if (!(dir.compare(".") == 0 || dir.compare("..") == 0))
			{
				processDirectory(getDirectoryName(path, dir));
			}
		}
	}
	catch (std::exception ex) {
		cout << "Exception" << ex.what();
	}
}

string DependencyAnalysisEngine::getDirectoryName(string base, string currentDirectory)
{
	string dirName = "";
	if (base.size() > 0 && currentDirectory.size() > 0) {
		if (base[base.length() - 2] == '\\' && base[base.length() - 3] == '\\')
			dirName = base + currentDirectory;
		else
			dirName = base + "\\\\" + currentDirectory;
	}
	return dirName;
}

//	ctor
DependencyAnalysisEngine::DependencyAnalysisEngine()
{
	this->namespaceContext = new list<string>;
	this->dependencies = new unordered_map<string, vector<string>>;
	this->typeTable = new TypeTable;
}

DependencyAnalysisEngine::~DependencyAnalysisEngine()
{
	this->namespaceContext->clear();
	this->namespaceContext = nullptr;

	this->dependencies->clear();
	this->dependencies = nullptr;

	this->typeTable = nullptr;
}

//	Process each file in the directory
void DependencyAnalysisEngine::processFiles(vector<string> files)
{
	for each (string fileName in files)
	{
		tokenizeAndProcessFile(fileName);
	}
}

//	Tokenize the file using the tokenizer.
void DependencyAnalysisEngine::tokenizeAndProcessFile(string fileName)
{
	this->namespaceContext->clear();	//	Clear scope for the file.
	queue<string> tokenQueue;
	std::ifstream in(fileName);
	if (!in.good()) { return; }
	Scanner::Toker toker;
	toker.returnComments(false);
	if (toker.attach(&in))
	{
		std::string token;
		try {
			do {
				string token = toker.getTok();
				if (token == "{" || token == ";" || token == "\n")
				{
					processTokens(tokenQueue, fileName);
					while (!tokenQueue.empty())
						tokenQueue.pop();
				}
				else
					tokenQueue.push(token);
			} while (in.good());
		}
		catch (exception ex) { cout << ex.what(); }
	}
}

//	Process tokens
void DependencyAnalysisEngine::processTokens(queue<std::string> tokenQueue, string fileName)
{
	if (tokenQueue.size() > 0) {
		processContext(tokenQueue);
		while (!tokenQueue.empty())
		{
			string token = tokenQueue.front();	//	Get the token.
			processDependency(token, fileName);
			tokenQueue.pop();
		}
	}
}

//	Maintain context of namespaces.
void DependencyAnalysisEngine::processContext(queue<string> tokenQueue)
{
	//	TODO: Rewrite algorithm
	while (!tokenQueue.empty())
	{
		string token = tokenQueue.front();
		if (token == "using") {
			tokenQueue.pop();
			string nextTok1 = tokenQueue.front();
			tokenQueue.pop();
			string nextTok2 = tokenQueue.front();
			tokenQueue.pop();
			if (nextTok1 == "namespace" && nextTok2 != "std") {
				while (!tokenQueue.empty()) {
					nextTok2 += tokenQueue.front();
					tokenQueue.pop();
				}
				namespaceContext->push_back(nextTok2);
			}
			else if (nextTok2 == "=") {
				//	alias names.
			}
		}
		if(!tokenQueue.empty())
			tokenQueue.pop();
	}
}

void DependencyAnalysisEngine::processDependency(string token, string fileName)
{
	list<Type> matchingTypes = this->typeTable->lookup(token, "");	//	Check if it is a type.
	if (matchingTypes.size() > 0) {
		Type * type = nullptr;
		Type type1;
		if (matchingTypes.size() > 1) {
			if (!namespaceContext->empty()) {
				for (auto iter = namespaceContext->begin(); iter != namespaceContext->end(); ++iter) {
					for (auto iter2 = matchingTypes.begin(); iter2 != matchingTypes.end(); ++iter2) {
						string ns = *iter, fqn = iter2->fullyQualifiedName;
						auto found = fqn.find(ns);
						if (found != std::string::npos) {
							type = &*iter2;
							break;
						}
					}
				}
			}
			else {
				for (auto iter2 = matchingTypes.begin(); iter2 != matchingTypes.end(); ++iter2)
				{
					if (fileName == iter2->packageName) {
						type = &*iter2;
						break;
					}
				}
			}
		}
		else
			type = &(matchingTypes.front());
		processDependency(type, fileName);
	}
}

void DependencyAnalysisEngine::processDependency(Type * type, string fileName) {
	if (type && type->typeName != "")
	{
		//	if token is defined in a different file add a dependency of this
		//	file onto that file.
		//	File = dependent, packageName = independent
		if (fileName != type->packageName) {
			if (type->packageName == "") { cout << "Culprit"; }
			auto iter = dependencies->find(fileName);	//	Find the file.
			if (iter != dependencies->end())
			{
				vector<string> vector = iter->second;
				if (std::find(vector.begin(), vector.end(), type->packageName) == vector.end())
					(iter->second).push_back(type->packageName);
			}
			else { //	Added into dependency table
				pair<string, vector<string>> dependency;
				dependency.first = fileName;						//	this depends on 
				dependency.second.push_back(type->packageName);		//	this
				this->dependencies->insert(dependency);
			}
		}
		//	If the token is present in the file where it is declared
		//	ignore it and move on.
		else {	/*cout << "dependency on same file."; */ }
	}
}

void DependencyAnalysisEngine::importTypeTable(TypeTable & table)
{
	*this->typeTable = table;
}

unordered_map<string, vector<string>> DependencyAnalysisEngine::exportAnalysisResult()
{
	return *this->dependencies;
}

//---------------------------TEST STUB----------------------------
//	Uncomment the following code to test DepAnalysisEngine Package
//
//	int main() {
//		DependencyAnalysisEngine engine;
//		engine.importTypeTable(typetable);		//	Import the type table.
//		engine.startDependencyAnalysis("C:\TestFiles");		//	Start dependency analysis
//		engine.exportAnalysisResultToFile(exec.getXMLPath(), exec.getAnalysisPath());
//		auto res = engine.exportAnalysisResult();
//		Loop and display results or interact with display module to print them.
//	}
//
