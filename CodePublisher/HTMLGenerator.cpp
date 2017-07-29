//////////////////////////////////////////////////////////////////////
// HTMLGenerator - Package is used for generating html, js and css	//
//				   files											//
// Ver 1.0															//
// Application: Dependency based Code Publisher						//
// Platform:    Lenovo Z580, Win 10 Pro, Visual Studio 2017 RC 1	//
// Author:      Kulkarni Rohit,										//
//              rvkulkar@syr.edu									//
//////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "HTMLGenerator.h"

using namespace std;

string HtmlGenerator::generateCSSFile()
{
	string css;
	css += "#dependencies {";
	css += " background-color: #c0c0c0; \n\n padding: 0 0 10px; ";
	css += "}";
	css += "\n\n";
	css += string(".button-expcol {\n   padding: 0 0 0px;\n") + "margin: 0 0 0px;\n"
		"border: 0px;\n" + "background-color: inherit;\n" + "cursor: pointer;\n}";
	css += "\n\n";
	css += string("pre { display:inline;\n margin: 0 0 0px;\n padding: 0 0 0px; }\n");
	css += "\n\n";
	css += "body { background-color: #d3d3d3; \n}\n\n";
	css += "";
	ofstream fileWriter(config.cssFileName, ios::out);
	if (fileWriter.is_open()) {
		fileWriter.write(css.c_str(), css.length());
	}
	return css;
}

void HtmlGenerator::generateJavascriptFile()
{
	string jsFileName = config.jsFileName;
	if (jsFileName.length() != 0) {
		size_t found = jsFileName.find(".js");
		if (found == string::npos)
			jsFileName.append(".js");
	}
	else jsFileName = "script.js";

	fstream fileWriter;
	fileWriter.open(jsFileName, ios::out);

	string javascript = "";
	javascript += "function showHideSection(division) { \nvar x = document.getElementById(division);\n";
	javascript += "\tif (x.style.display == 'none') {\n";
	javascript += "\t\tx.style.display = 'inline'; \n}\n";
	javascript += "\telse {\n";
	javascript += "\t\tx.style.display = 'none'; \n\t} \n}\n";
	javascript += "\n\n";
	fileWriter.write(javascript.c_str(), javascript.length());
	fileWriter.close();
}

string HtmlGenerator::addDependencies(string fileName,
	unordered_map<string, vector<string>> dependencies)
{
	string htmlString = createTag("div", true, " id=\"dependencies\"");
	htmlString += createTag("h3");
	htmlString += "Dependencies";
	htmlString += createTag("h3", false);
	for (auto dep : dependencies) {
		if (dep.first == fileName) {
			for (auto file : dep.second) {
				string fileInfo = file + ".html";
				string dependency = "<a href = \"";
				dependency += fileInfo;
				dependency += "\">";
				dependency += file;
				dependency += "</a>";
				htmlString += dependency;
				htmlString += "<br />";
			}
		}
	}
	htmlString += createTag("div", false);
	return htmlString;
}

HtmlGenerator::HtmlGenerator()
{
	config.cssFileName = "stylesheet.css";
	config.jsFileName = "script.js";
}

HtmlGenerator::HtmlGenerator(Config c)
{
	this->config = c;
}

string HtmlGenerator::createHtmlPage(string fileName, string data,
	unordered_map<string, vector<string>> dependencies)
{
	string html = "";
	html += createTag("!DOCTYPE html");
	html += createTag("html");
	html += createTag("head");
	html += addScriptLink();
	html += createTag("head", false).append("\n");
	html += createTag("body");
	html += createTag("h2");
	html += fileName;
	html += createTag("h2", false) + "\n";
	html += addDependencies(fileName, dependencies);
	html += createTag("hr");
	html += createTag("pre");
	html += injectJavascriptFunctionality(data);
	html += createTag("pre", false);
	html += createTag("hr");
	html += createTag("body", false);
	html += createTag("html", false);
	return html;
}

string HtmlGenerator::injectJavascriptFunctionality(string data)
{
	int counter = 0, i = 0;
	string span = "span", ctr = "";
	while (data[i] != '\0') {
		if (data[i] == '{') {
			if (i < data.length() && data[i + 1] == '}') {
				i = i + 2;
				continue;
			}
			stringstream ss;
			ss << counter++;
			ss >> ctr;
			ctr = span + ctr;
			string preInject = "</pre><button class=\"button-expcol\" onclick=\"showHideSection('" + ctr + "')\">";
			string postInject = "</button><br /><div id=\"" + ctr + "\" style = \"display: inline\"><pre>";
			data.insert(i + 1, postInject);
			data.insert(i, preInject);
			i += preInject.length() + postInject.length();
		}
		else if (data[i] == '}') {
			string preInject = "</pre></div><pre>";
			int index = i - 1;
			while (index > 0) {
				if (data[index] == '\t') {
					index--;
					continue;
				}
				else if (data[index] == ' ') {
					index--;
					continue;
				}
				else
					break;
			}
			data.insert(index + 1, preInject);
			i += preInject.length();
		}
		i++;
	}
	return data;
}

string HtmlGenerator::createTag(const string data, bool open, string attributeInfo)
{
	string tag = "<";
	tag += open ? "" : "/";
	tag += data;
	tag += attributeInfo;
	tag += ">\n";
	return tag;
}

string HtmlGenerator::addScriptLink()
{
	string link = "";
	link += "<script type=\"text/javascript\" src=\"" + config.jsFileName + "\">" + createTag("script", false);
	link += "<link rel=\"stylesheet\" href=\"" + config.cssFileName + "\">" + createTag("link", false);
	return link;
}

void HtmlGenerator::operator()(Config c)
{
	if (c.cssFileName.find(".css") == string::npos) {
		c.cssFileName += ".css";
	}
	if (c.jsFileName.find(".js") == string::npos) {
		c.jsFileName += ".js";
	}
	config = c;
}

//------------------------------TEST STUB---------------------------
//	Uncomment the following code to test HTMLGenerator package
//	int main() {
//		Config config;
//		config.cssFileName = "styleSheet.css";
//		config.jsFileName = "scr.js";
//		HtmlGenerator generator(config);
//		generator.generateCSSFile();
//		generator.generateJavascriptFile();
//		generator.createHtmlPage("myfile.cpp", "...", dependencies);
//  }