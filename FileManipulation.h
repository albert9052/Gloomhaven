#pragma once

#include <io.h>
#include <string>
#include <vector>

using namespace std;

// Get all the files within the directory
// path (string): the path of the directory you want to search
// files (vector<string>&): it will push all the files' name in this vector
void getFiles(string path, vector<string>& files)
{
    //檔案控制代碼
    long hFile = 0;
    //檔案資訊  
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            //如果是目錄,迭代之  
            //如果不是,加入列表  
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            else
            {
                files.push_back(p.assign(fileinfo.name));
            }

        } while (_findnext(hFile, &fileinfo) == 0);

        _findclose(hFile);
    }
}

// Get all the .txt files within the directory
// path (string): the path of the directory you want to search
// files (vector<string>&): it will push all the .txt files' name in this vector
void getTxtFiles(string path, vector<string>& files)
{
    vector<string> tempVector;
    getFiles(path, tempVector);
    for (int i = 0; i < tempVector.size(); i++)
    {
        if (tempVector[i].find(".txt", 0) == string::npos)
        {
            tempVector.erase(tempVector.begin() + i);
        }
    }
    for (string str : tempVector)
    {
        files.push_back(str);
    }
}