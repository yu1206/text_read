#include <iostream>
#include <fstream>
#include <string>
#include <vector>
 #include <sstream> 
#include <io.h>
#include <Windows.h>
#include "opencv\cv.h"
#include "opencv\highgui.h"
using namespace cv;
 using namespace std;
 int read_pos_file_x2y2(string str_pos,string str_pos_x2y2,map<string,vector<Rect>>& pos)
 {
	 fstream f_pos_x2y2;
	 f_pos_x2y2.open(str_pos_x2y2, ios::in);
	 if (!f_pos_x2y2.is_open())
	 {
		 cout << "文件数据打开错误！"<< endl;
		 system("pause");
		 return false;
	 }

	 

	 char line[256];
	 while(f_pos_x2y2.getline(line,sizeof(line)))
	 {
		 string line_name; 
		 Rect rt;
		 int x1,y1,x2,y2;
		 stringstream str_s(line);
		 str_s >> line_name>>x1>>y1>>x2>>y2;

			rt.x=x1;rt.y=y1;rt.width =x2-x1+1;rt.height =y2-y1+1;
		 pos[line_name].push_back(rt);
	 }

	 f_pos_x2y2.close();
 }
 int read_pos_file(string str_pos,string str_pos_x2y2,map<string,vector<Rect>>& pos)
 {
	 fstream f_pos;
	 f_pos.open(str_pos, ios::in);
	 if (!f_pos.is_open())
	 {
		 cout << "文件数据打开错误！"<< endl;
		 system("pause");
		 return false;
	 }

	 fstream f_pos_x2y2;
	 f_pos_x2y2.open(str_pos_x2y2, ios::trunc | ios::out);
	 if (!f_pos_x2y2.is_open())
	 {
		 cout << "x2y2文件数据打开错误！"<< endl;
		 system("pause");
		 return false;
	 }

	 char line[256];
	 while(f_pos.getline(line,sizeof(line)))
	 {
		 string line_name; 
		 Rect rt;
		 stringstream str_s(line);
		 str_s >> line_name>>rt.x>>rt.y>>rt.width>>rt.height;

		 f_pos_x2y2<<line_name.c_str()<<" "<<rt.x<<" "<<rt.y<<" "<<rt.x+rt.width-1<<" "<<rt.y+rt.height-1<<endl;

		 pos[line_name].push_back(rt);
	 }

	 f_pos.close();
	 f_pos_x2y2.close();
 }
 extern int readDir(char *dirName, vector<string> &filesName);
 int test6(int argc, char *argv[])
 {
	 string str_image="I:/calibration/0_乌鲁木齐800/1";
	   string str_save="I:/calibration/0_乌鲁木齐800/save";
	 string str_compare="I:/calibration/0_乌鲁木齐800/0_vehicle_less.txt";
	 fstream f_compare;
	 f_compare.open(str_compare, ios::out);
	 if (!f_compare.is_open())
	 {
		 cout << "文件数据打开错误1！"<< endl;
		 system("pause");
		 return false;
	 }

	 string str_vehicle_pos="I:/calibration/0_乌鲁木齐800/0_vehicle_rect.txt";
	 string str_vehicle_pos_x2y2="I:/calibration/0_乌鲁木齐800/0_vehicle_rect_x2y2.txt";
	 map<string,vector<Rect>> vehicle_pos;
	 read_pos_file(str_vehicle_pos,str_vehicle_pos_x2y2,vehicle_pos);

	 string str_plate_pos="I:/calibration/0_乌鲁木齐800/0_plate_rect.txt";
	 string str_plate_pos_x2y2="I:/calibration/0_乌鲁木齐800/0_plate_rect_x2y2.txt";
	 map<string,vector<Rect>> plate_pos;
	 read_pos_file(str_plate_pos,str_plate_pos_x2y2,plate_pos);


	 vector<string> filesName;
	 readDir((char*)str_image.c_str(),filesName);
	 for(int i=0;i<filesName.size();i++)
	 {
		 string fullName=filesName[i];
		 Mat img=imread(fullName.c_str());
		 if(img.data==NULL)
		 {
			 printf("img.data=NULL\n");
			 system("pause");
			 continue;
		 }

		 int pos=fullName.find_last_of('/');
		 int pos2=fullName.find_first_of('.');
		 string name=fullName.substr(pos+1,pos2-pos-1);

		 vector<Rect> vehicle_rect;
		 map<string,vector<Rect>>::iterator vehicle_key = vehicle_pos.find(name);  
		 if(vehicle_key!=vehicle_pos.end())  
		 {  
			 vehicle_rect=vehicle_key->second;
		 }  

		 vector<Rect> plate_rect;
		 map<string,vector<Rect>>::iterator plate_key = plate_pos.find(name);  
		 if(vehicle_key!=vehicle_pos.end())  
		 {  
			 plate_rect=vehicle_key->second;
		 }  

		 if(vehicle_rect.size()!=plate_rect.size())
		 {
			 printf("车辆个数与车牌个数不等");
		      f_compare<<name.c_str()<<endl;
		 }

		 for(int j=0;j<vehicle_rect.size();j++)
		 {
			 Rect rt=vehicle_rect[j];
			 rectangle(img,rt,cvScalar(0,255,0));

			 stringstream ss;
			 ss<<j; 
			 string s1 = ss.str();
			 Point pt(rt.x+10,rt.y+10);
			 putText(img,s1,pt,CV_FONT_HERSHEY_DUPLEX,1.0f,cvScalar(0,255,0));
		 }
		 for(int j=0;j<plate_rect.size();j++)
		 {
			 Rect rt=plate_rect[j];
			 rectangle(img,rt,cvScalar(0,0,255));

			 stringstream ss;
			 ss<<j; 
			 string s1 = ss.str();
			 Point pt(rt.x+10,rt.y+10);
			 putText(img,s1,pt,CV_FONT_HERSHEY_DUPLEX,1.0f,cvScalar(0,0,255));
		 }
		 string path= str_save+"/"+ name+".jpg";
		 imwrite(path.c_str(),img);
	 }

	 return 0;
 }

 int test6_1(int argc, char *argv[])
 {
	 string str_image="I:/calibration/0_乌鲁木齐800/1";
	   string str_save="I:/calibration/0_乌鲁木齐800/save";
	 string str_compare="I:/calibration/0_乌鲁木齐800/0_vehicle_less.txt";
	 fstream f_compare;
	 f_compare.open(str_compare, ios::out);
	 if (!f_compare.is_open())
	 {
		 cout << "文件数据打开错误1！"<< endl;
		 system("pause");
		 return false;
	 }

	// string str_vehicle_pos="I:/calibration/0_乌鲁木齐800/0_vehicle_rect.txt";
		string str_vehicle_pos;
	 string str_vehicle_pos_x2y2="I:/calibration/0_乌鲁木齐800/0_vehicle_rect_x2y2.txt";
	 map<string,vector<Rect>> vehicle_pos;
	 read_pos_file_x2y2(str_vehicle_pos,str_vehicle_pos_x2y2,vehicle_pos);

	 string str_plate_pos="I:/calibration/0_乌鲁木齐800/0_plate_rect.txt";
	 string str_plate_pos_x2y2="I:/calibration/0_乌鲁木齐800/0_plate_rect_x2y2.txt";
	 map<string,vector<Rect>> plate_pos;
	 read_pos_file(str_plate_pos,str_plate_pos_x2y2,plate_pos);


	 vector<string> filesName;
	 readDir((char*)str_image.c_str(),filesName);
	 for(int i=0;i<filesName.size();i++)
	 {
		 string fullName=filesName[i];
		 Mat img=imread(fullName.c_str());
		 if(img.data==NULL)
		 {
			 printf("img.data=NULL\n");
			 system("pause");
			 continue;
		 }

		 int pos=fullName.find_last_of('/');
		 int pos2=fullName.find_first_of('.');
		 string name=fullName.substr(pos+1,pos2-pos-1);

		 vector<Rect> vehicle_rect;
		 map<string,vector<Rect>>::iterator vehicle_key = vehicle_pos.find(name);  
		 if(vehicle_key!=vehicle_pos.end())  
		 {  
			 vehicle_rect=vehicle_key->second;
		 }  

		 vector<Rect> plate_rect;
		 map<string,vector<Rect>>::iterator plate_key = plate_pos.find(name);  
		 if(plate_key!=plate_pos.end())  
		 {  
			 plate_rect=plate_key->second;
		 }  

		 if(vehicle_rect.size()!=plate_rect.size())
		 {
			 printf("车辆个数与车牌个数不等\n");
		      f_compare<<name.c_str()<<endl;
		 }

		 for(int j=0;j<vehicle_rect.size();j++)
		 {
			 Rect rt=vehicle_rect[j];
			 rectangle(img,rt,cvScalar(0,255,0));

			 stringstream ss;
			 ss<<j; 
			 string s1 = ss.str();
			 Point pt(rt.x+10,rt.y+10);
			 putText(img,s1,pt,CV_FONT_HERSHEY_DUPLEX,1.0f,cvScalar(0,255,0));
		 }
		 for(int j=0;j<plate_rect.size();j++)
		 {
			 Rect rt=plate_rect[j];
			 rectangle(img,rt,cvScalar(0,0,255));

			 stringstream ss;
			 ss<<j; 
			 string s1 = ss.str();
			 Point pt(rt.x+10,rt.y+10);
			 putText(img,s1,pt,CV_FONT_HERSHEY_DUPLEX,1.0f,cvScalar(0,0,255));
		 }
		 string path= str_save+"/"+ name+".jpg";
		 imwrite(path.c_str(),img);
	 }

	 return 0;
 }