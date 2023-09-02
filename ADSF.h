#pragma once
#include"D:\CppHeader\winapicore.h"
#include"D:\CppHeader\7zipAPI.h"
#include"D:\CppHeader\GlobalVar.h"
using namespace std;

string what = "HelloWord";

string RunINFO = getselfinfo();
string RunPath = getselfpath();

string PubVar = RunPath + "\\vartemp~" + to_string(SpawnRandomNum(1111, 9999)) + ".ini";

string cpause(string Notices) {
	cout << Notices;
	system("pause >nul");
	return what;
}

int CountLines(string filename)
{
	ifstream ReadFile;
	int n = 0;
	string tmp;
	ReadFile.open(filename.c_str());//ios::in ��ʾ��ֻ���ķ�ʽ��ȡ�ļ�
	if (ReadFile.fail())//�ļ���ʧ��:����0
	{
		return 0;
	}
	else//�ļ�����
	{
		while (getline(ReadFile, tmp, '\n'))
		{
			n++;
		}
		ReadFile.close();
		return n;
	}
}

string LineReader(string File, int line_number) {
		int lines, i = 0;
		string temp;
		fstream file;
		file.open(File.c_str());
		lines = CountLines(File);

		if (line_number <= 0)
		{
			return "Line Error";
		}
		if (file.fail())
		{
			return "Error File not exist";
		}
		if (line_number > lines)
		{
			return "overline";
		}
		while (getline(file, temp) && i < line_number - 1)
		{
			i++;
		}
		file.close();
		return temp;
}

void WriteIntGlobal(string GlobalData, int intdata) {
	string IntNum = to_string(intdata);
	WriteGlobal(GlobalData, IntNum);
}
int GetIntGlobal(string GlobalData) {
	int RetIntd;
	string GETG = GetGlobal(GlobalData);
	RetIntd = atoi(GETG.c_str());
	return RetIntd;
}

string CleanAuto(string info, string replaceword) {
	return Replace(info, replaceword, "");
}

string cutlineblock(string lines, int line) {
	string randfilename = RunPath +  "\\cget" + to_string(SpawnRandomNum(1111, 9999)) + ".ini";

	string CoreFile = RunPath + "\\TransLine.exe";
	string parmaset = randfilename + " " + lines;
	if (_access(CoreFile.c_str(), 0)) {
		cout << "TransLine Missing, Please try reinstall Access Center" << endl;
		cout << "File Path :  " << CoreFile << endl;
		cout << "ERROR" << endl;
		return "TLError";
	}
	ShellExecute(0, "open", CoreFile.c_str(), parmaset.c_str(), 0, SW_HIDE);

	ReCheckFile:
	Sleep(100);

	string readinis = "P" + to_string(line);

	if (_access(randfilename.c_str(), 0)) {
		goto ReCheckFile;
	}
	string retinfo = readini(randfilename, "TransLine", readinis);
	
	remove(randfilename.c_str());

	if (retinfo == "readini-failed") {
		return "NULL";
	}
	return retinfo;
}

string TransVar(string Info) {
	if (_access(PubVar.c_str(), 0)) {
		return Info;
	}
	string heronum = "TransVar~T";
	string herodata = "TransVar~D";
	WriteIntGlobal(heronum, 1);
	WriteGlobal(herodata, Info);

BackGet:
	int localget = GetIntGlobal(heronum);
	string geti = "MRA" + to_string(localget);

	string gets = readini(PubVar, "MRALIST", geti);
	
	string GETSMax = readini(PubVar, "MRALIST", "$maxnum");
	int gsmax_int = atoi(GETSMax.c_str());

	if (gets > GETSMax) {
		string herocha = GetGlobal(herodata);
		if (herocha == "readini-failed") {
			return "";
		}
		return herocha;
	}

	string gvars = readini(PubVar, "VarST", gets);

	string herocha = GetGlobal(herodata);
	WriteGlobal(herodata,Replace(herocha, gets, gvars));

	localget++;
	WriteIntGlobal(heronum, localget);
	goto BackGet;
}

int GetMaxNum(string Dict, string Head) {
	string RTCode = readini(Dict, Head, "$maxnum");
	int ReturnINFO;
	ReturnINFO = atoi(RTCode.c_str());
	return ReturnINFO;
}
int WriteMaxNum(string Dict, string Head, int maxnum) {
	string maxnumse = to_string(maxnum);
	writeini(Dict, Head, "$maxnum", maxnumse);
	return maxnum;
}
string WriteNewMRA(string Dict, string Head, string INFO) {
	int getmaxnum = GetMaxNum(Dict, Head);
	getmaxnum++;
	WriteMaxNum(Dict, Head, getmaxnum);
	string gmnstr = to_string(getmaxnum);
	string NewMRA = "MRA" + gmnstr;
	writeini(Dict, Head, NewMRA, INFO);
	return NewMRA;
}

//�����˳���� 0.�쳣��� 1
int ScriptRun(string File) {
	//��ʼ������
	ofstream logs;
	logs.open("open.log");
	string EPoint = "ADSF~para";
	WriteIntGlobal(EPoint, 2);

	//StartRun
	system("cls");

	logs << "start run" << endl;
	logs << "Script :  " << File << endl;

BackRoll:
	//��ȡ����
	int CURRLINE = GetIntGlobal(EPoint);
	string cl_str = to_string(CURRLINE);

	//cout << cl_str << endl;

	string GetLineData = LineReader(File, CURRLINE);
	string getlineinfo = TransVar(GetLineData);
	logs << "Line :  " << cl_str << " . INFO :  " << getlineinfo << endl;
	

	//cout << getlineinfo << endl;

	if (int a = checkChar(getlineinfo, "///") == 1) {
		logs << "Skip" << endl;
		CURRLINE++;
		WriteIntGlobal(EPoint, CURRLINE);
		goto BackRoll;	
	}
	if (int a = checkChar(getlineinfo, "$$Prints") == 1) {
		//����ĵ�
		string p1 = CleanAuto(getlineinfo, ";");
		string p2 = CleanAuto(p1, "\"");
		string p3 = CleanAuto(p2, "(");
		string p4 = CleanAuto(p3, ")");
		string out = CleanAuto(p4, "$$Prints");
		printf(out.c_str());
		logs << "Print :  " << out << endl;
		cout << endl;
		CURRLINE++;
		WriteIntGlobal(EPoint, CURRLINE);
		goto BackRoll;
	}
	if (int a = checkChar(getlineinfo, "$$Cout") == 1) {
		//����ĵ�
		string p1 = CleanAuto(getlineinfo, ";");
		string p2 = CleanAuto(p1, "\"");
		string p3 = CleanAuto(p2, "(");
		string p4 = CleanAuto(p3, ")");
		string out = CleanAuto(p4, "$$Cout");
		printf(out.c_str());
		logs << "Cout :  " << out << endl;
		cout << endl;
		CURRLINE++;
		WriteIntGlobal(EPoint, CURRLINE);
		goto BackRoll;
	}
	if (int a = checkChar(getlineinfo, "$$endl") == 1) {
		cout << endl;
		logs << "Endl Line" << endl;
		CURRLINE++;
		WriteIntGlobal(EPoint, CURRLINE);
		goto BackRoll;
	}
	if (int a = checkChar(getlineinfo, "$Windows.CMD") == 1) {
		//Windows CMD
		string p1 = CleanAuto(getlineinfo, ";");
		string p2 = CleanAuto(p1, "\"");
		string p3 = CleanAuto(p2, "(");
		string p4 = CleanAuto(p3, ")");
		string out = CleanAuto(p4, "$Windows.CMD");
		system(out.c_str());
		logs << "Run Windows Command :  " <<out << endl;
		CURRLINE++;
		WriteIntGlobal(EPoint, CURRLINE);
		goto BackRoll;
	}
	if (int a = checkChar(getlineinfo, "$end") == 1) {
		logs << "Script End Task" << endl;
		remove(PubVar.c_str());
		return 0;
	}
	if (int a = checkChar(getlineinfo, "$$SetConTitle") == 1) {
		//����ĵ�
		string p1 = CleanAuto(getlineinfo, ";");
		string p2 = CleanAuto(p1, "\"");
		string p3 = CleanAuto(p2, "(");
		string p4 = CleanAuto(p3, ")");
		string out = CleanAuto(p4, "$$SetConTitle");
		SetConsoleTitle(out.c_str());
		logs << "Set Console Title :  " <<out << endl;
		CURRLINE++;
		WriteIntGlobal(EPoint, CURRLINE);
		goto BackRoll;
	}
	if (int a = checkChar(getlineinfo, "$$Timeout") == 1) {
		//����ĵ�
		string p1 = CleanAuto(getlineinfo, ";");
		string p3 = CleanAuto(p1, "(");
		string p4 = CleanAuto(p3, ")");
		string out = CleanAuto(p4, "$$Timeout");
		int sleepout = atoi(out.c_str());
		logs << "Timeout (ms) :  " << out << endl;
		Sleep(sleepout);
		
		CURRLINE++;
		WriteIntGlobal(EPoint, CURRLINE);
		goto BackRoll;
	}
	if (int a = checkChar(getlineinfo, "$$CleanCon") == 1) {
		system("cls");
		logs << "Cls Console" << endl;
		CURRLINE++;
		WriteIntGlobal(EPoint, CURRLINE);
		goto BackRoll;
	}

	//WindowsAPI

	//�����ļ���
	if (int a = checkChar(getlineinfo, "$$winapi.md") == 1) {
		string p1 = CleanAuto(getlineinfo, ";");
		string p2 = CleanAuto(p1, "\"");
		string p3 = CleanAuto(p2, "(");
		string p4 = CleanAuto(p3, ")");
		string out = CleanAuto(p4, "$$winapi.md");

		CreateDirectory(out.c_str(), 0);
		logs << "Create Folder :  " << out << endl;

		CURRLINE++;
		WriteIntGlobal(EPoint, CURRLINE);
		goto BackRoll;
	}
	if (int a = checkChar(getlineinfo, "$$winapi.rd") == 1) {
		string p1 = CleanAuto(getlineinfo, ";");
		string p2 = CleanAuto(p1, "\"");
		string p3 = CleanAuto(p2, "(");
		string p4 = CleanAuto(p3, ")");
		string out = CleanAuto(p4, "$$winapi.rd");

		rmfolder(out.c_str());

		logs << "Remove Folder :  " << out << endl;

		CURRLINE++;
		WriteIntGlobal(EPoint, CURRLINE);
		goto BackRoll;
	}
	if (int a = checkChar(getlineinfo, "$$winapi.urlmon") == 1) {
		logs << "URLDownload" << endl;
		string p1 = CleanAuto(getlineinfo, ";");
		string p2 = Replace(p1, "\",\""," ");
		string p3 = CleanAuto(p2, "(");
		string p4 = CleanAuto(p3, ")");
		string p5 = CleanAuto(p4, "\"");
		string out = CleanAuto(p5, "$$winapi.urlmon");

		string URLINFO = cutlineblock(out, 1);
		string SPINFO = cutlineblock(out, 2);
		string SWITCHS = cutlineblock(out, 3);

		logs << "URL :  "  << URLINFO << endl;
		logs << "Save Path :  " <<SPINFO << endl;
		logs << "SWITCHS :  " << SWITCHS << endl;
		if (SWITCHS == "hide") {
			URLDown(URLINFO, SPINFO);
		}
		else {
			cout << endl;
			cout << "Downloading File :   " << URLINFO << endl;
			cout << "Save Path :  " << SPINFO << endl;
			URLDown(URLINFO, SPINFO);
		}
		if (_access(SPINFO.c_str(), 0)) {
			logs << "Download Failed" << endl;
			cout << "Download Failed" << endl;
		}
		else {
			logs << "Download OK" << endl;
			cout << "Download OK" << endl;
		}
		logs << "Complete" << endl;

		CURRLINE++;
		WriteIntGlobal(EPoint, CURRLINE);
		goto BackRoll;
	}
	if (int a = checkChar(getlineinfo, "$$var") == 1) {

		string p1 = CleanAuto(getlineinfo, ";");
		string p2 = Replace(p1, "\",\"", " ");
		string p3 = CleanAuto(p2, "(");
		string p4 = CleanAuto(p3, ")");
		string p5 = CleanAuto(p4, "\"");
		string out = CleanAuto(p5, "$$var");

		string varname = cutlineblock(out, 1);
		string varinfo = cutlineblock(out, 2);

		WriteNewMRA(PubVar, "MRALIST", varname);
		writeini(PubVar, "VarST", varname, varinfo);

		CURRLINE++;
		WriteIntGlobal(EPoint, CURRLINE);
		goto BackRoll;
	}

	//��Ч��������
	if (getlineinfo == "overline") {
	    	logs << "Over Line Error" << endl;
	     	remove(PubVar.c_str());
		    cout << endl;
			cout << "Script Not Set Exist API" << endl;
			return 1;
	}
	if (getlineinfo == "") {
		CURRLINE++;
		WriteIntGlobal(EPoint, CURRLINE);
		goto BackRoll;
	}

	//δָ֪��
	logs << "Unknown Command :  " << getlineinfo << endl;
	cout << endl;
	cout << "_" << getlineinfo << "_ is not a command." << endl;
	cout << "Please Check This Line is Right ? " << endl;
	cout << endl;
	CURRLINE++;
	WriteIntGlobal(EPoint, CURRLINE);
	goto BackRoll;
}