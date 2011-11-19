#include <iostream>
#include <fstream>
#include <string>

using namespace std;


bool fexists(string filename) {
	ifstream ifile(filename.c_str());
	return ifile;
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cout << argv[0] << " v2.0:" << endl << "\tMerge XML files to std output or an output file."<<endl<<"\tALL XML should have the same xml prolog and root node." << endl;
		cout << endl << "Syntax:" <<endl << "\t" << argv[0] << " PARAMS <XML_FILE> <XML_FILE> ..." << endl;
		cout << endl << "Parameters:"<<endl;
		cout << endl << "\t-o<OUTPUT_XML_FILE>\tPath of the XML output file." << endl;
		cout << endl << "\t-p[ParentRootName]\tAdd a parent root. If ParentRootName is not provided,\n\t\t\t\tthe name of a root postfixed with an 's' is used." << endl;
		cout << endl << "Samples:"<<endl;
		cout << endl << "\tmergex *.xml"<<endl;
		cout << "\tmergex a.xml b.xml c.xml -ooutput.xml"<<endl;
		cout << "\tmergex *.xml -p -ooutput.xml"<<endl;
		cout << "\tmergex *.xml -pROOT"<<endl;
		cout << "\tmergex -ooutput.xml -pROOT a.xml b.xml c.xml"<<endl;
		return 0;
	}

	bool createParent(false);
	string outputFilePath,firstXMLFilePath,parentRootName;
	for (int i=1; i<argc; i++) {
		string param(argv[i]);
		createParent = createParent || param.compare(0, 2, "-p") == 0;
		if (param.compare(0, 2, "-p") == 0) {
			parentRootName = param.substr(2);
		} else if (param.compare(0, 2, "-o") == 0) {
			outputFilePath = param.substr(2);
		} else if (firstXMLFilePath.empty() && fexists(param)) {
			firstXMLFilePath = param;
		}
	}

	bool outputToFile = !outputFilePath.empty();

	ifstream firstXML(firstXMLFilePath.c_str());
	if (firstXML) {
		bool found(false);
		char c;
		string rootName,header;
		while (firstXML.good() && !found) {
			firstXML.get(c);
			header += c;
			found = c=='<';
			if (found && firstXML.good()) {
				firstXML.get(c);
				header += c;
				found = c!='!' && c!='?';
				if (found) {
					rootName += c;
					bool foundSpace(false);
					bool foundEndRootNode(false);
					while (firstXML.good() && !foundEndRootNode) {
						firstXML.get(c);
						header += c;
						foundSpace = foundSpace || c==' ' || c=='\t' || c=='>' || c=='/';
						if (!foundSpace) {
							rootName += c;
						}
						foundEndRootNode = c=='>';
					}
				}
			} else { found = false; }
		}
		firstXML.close();

		if (found) {
			string startRootNodeKey("<"+rootName);
			string endRootNodeKey("</"+rootName);
			if (parentRootName.empty()) {
				parentRootName = rootName+"s";
			}

			string::size_type pos;
			if ((pos = header.find("/>")) != string::npos) {
				header.replace(pos, 1, "");
			}
			if (createParent) {
				string::size_type pos;
				if ((pos = header.find(startRootNodeKey)) != string::npos) {
					header = header.substr(0,pos)+"<"+parentRootName+">";
				}
			}

			ofstream fout(outputFilePath.c_str());
			ostream& out = ((outputToFile) ? fout : cout);
			if (outputToFile) {
				cout <<"Merging "<<rootName<<"(s) into "<<outputFilePath<<endl;
			}
			out <<header;
			for (int i=1; i<argc; i++) {
				string inputFilePath(argv[i]);
				ifstream otherXML(inputFilePath.c_str());
				if (otherXML) {
					string line,xml;
					while (otherXML.good()) {
						getline(otherXML,line);
						xml.append(line);
						if (otherXML.good()) xml.append("\n");
					}
					otherXML.close();
					string::size_type found;
					found = xml.find(startRootNodeKey);
					if (found != string::npos) {
						if (createParent) {
							xml.replace(0,found,"");
						} else {
							string::size_type endOfTag = xml.find(">",found);
							if (endOfTag != string::npos){
								xml.replace(0,endOfTag+1,"");
							}
						}
					}

					if (!createParent) {
						found = xml.rfind(endRootNodeKey);
						if (found != string::npos) {
							xml.replace(found,xml.length() - found,"");
						}
					}
					out <<xml;
				}
			}

			string endRootTag("</"+rootName+">");
			if (createParent) {
				endRootTag = "</"+parentRootName+">";
			}
			out <<endl<<endRootTag<<endl;
			if (ofstream * ofs = static_cast<ofstream *>(&out)) {
				ofs->close();
			}
		} else {
			cerr <<"ERROR: first xml not valid"<<endl;
		}
	}

	return 0;
}

