//
// Created by kxw on 3/15/17.
//

#ifndef SORT_GROUPING_MERGESORT_H
#define SORT_GROUPING_MERGESORT_H
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <stdint.h>
using namespace std;

/*
 *
 */
struct kv{
    string k;
    string v;
};
string  get_id(struct kv t){

    return t.k.c_str();
}
string get_id_from_file(string t){
    int index = t.find_first_of("\t",0);
    string id  = t.substr(0,index);//将string转化成int
    return id;
}
double get_score(string line){
    int index = line.find_first_of("\t",0);
    double score  = atof(line.substr(index+1).c_str());
    return score;
}
string inttostring(int he){
    char t[256];
    string s;

    sprintf(t, "%d", he);
    s = t;
    return s;
}

void merge_sort_buffer(int in_file_sum,string in_dir,string out_file,int EveMergeFileBufferNum){
    if(in_file_sum<=1||EveMergeFileBufferNum<=1) return;
    //归并临时文件
    ifstream  * farray = new ifstream[in_file_sum];//打开多文件输入的句柄
    bool *hasnoNext = new bool[in_file_sum];//标记文件是否读完
    ofstream fout;
    fout.open(out_file.c_str(),ios::app|ios::ate);
    string * * data = new string *[in_file_sum];
    int cursor[in_file_sum];
    int capacity[in_file_sum];
    for(int k =0;k<in_file_sum;k++){
        data[k] =new string[EveMergeFileBufferNum];
        cursor[k] =0;
        capacity[k] =EveMergeFileBufferNum;
    }
    for(int i=0;i<in_file_sum;i++){
        farray[i].open((in_dir+inttostring(i)).c_str(),ios::in);//为每一个文件指针打开文件

        if(!farray[i].is_open()){
            cout<<"文件"<<i<<"没有正确打开！"<<endl;
            exit(0);
        }
        int count =0;
        while(count<EveMergeFileBufferNum) {
            if (!getline(farray[i], data[i][count++]))//从当前文件中读一行放到对应文件最小数据的数组中
                ///如果当前文件已经读完了，也就是说是一个空文件，那么farray[i].eof()应该为true
            {
                hasnoNext[i] = true;//说明当前文件中包含的数据量不足以填满缓冲区,标记当前文件中的数据已经完全在缓冲区了
                capacity[i] = count;//将容量缩小到实际的数值
                break;
                //cout<<"hasnoNext["<<i<<"]="<<hasnoNext[i]<<endl;
            }//标记该文件是空的
            //  cout<<"读出文件"<<i<<"中数据放入 data["<<i<<"]="<<data[i]<<endl;
        }
    }//初始化文件指针和缓存区
    int j=0;//用来记录最后排序剩下的那个文件的下标。
    while(true){//!hasnoNext[i]表示文件中还有数据没加载到缓冲区，hasnoNext[i]表示完全加载到缓冲区，
                //capacity[i]<EveMergeFileBufferNum 表示缓冲区中数据不足以填满缓冲区，cursor[i]==capacity[i]表示缓存区中没有下一个元素了
        string min; //最小元素
        bool notstop = false;//停止标志

        for(int i=0;i<in_file_sum;i++){//找到第一个非空的假设最小值
            if(cursor[i]<capacity[i])//有下一个，也就是在当前缓冲区中还有没排序的元素
            {min = data[i][cursor[i]];
                cursor[i]++;//游标增1，因为不确定当前第一个非空元素是不是最小元素，是否要写到结果文件中，所以游标先不增1
                j= i;//标记当前第一个非空元素所在的文件号j，跳出循环
                break;
            }
        }
        for(int i = j+1;i<in_file_sum;i++){//循环找出最小的元素，交给min，并记录到下标在j
            if(cursor[i]<capacity[i]&&get_id_from_file(data[i][cursor[i]])<get_id_from_file(min)){//缓冲区中还有没处理的元素，防止下标越界
                min = data[i][cursor[i]];
                cursor[i]++;//因为不确定当前元素是不是最小元素，是否要写到结果文件中，所以游标先不增1
                cursor[j]--;//上一轮的最小元素被替换了，上一轮的游标退回一格。
                j =i;
            }
            if(!hasnoNext[i]||cursor[i]<capacity[i]) notstop =true;//说明文件中还有元素，或者缓冲区中还有元素，stop置为true
        }
        fout<<min<<endl;
        //cout<<"从文件"<<j<<"找出最小的元素放入文件："<<min<<endl;
        fout.flush();
        if(!notstop) {//不停止标记为false，即停止！
            // cout<<"跳出循环，终止排序"<<endl;
            break;
        }
        if(cursor[j]==capacity[j]) {////j用来记录最小元素的文件指针下标，当文件j的最小元素被写出到排序文件后，查看现在文件j的缓存是不是空了，
            //也就是游标等于缓存上限了，是空了的话，那么就重新从文件j中读取出EveMergeFileBufferNum条记录
            //&&!getline(farray[j],data[j]))//从当前文件中读一行放到对应文件最小数据的数组中
            ///如果当前文件已经读完了，也就是说是一个空文件，那么farray[i].eof()应该为true
            int count =0;
            while(count<EveMergeFileBufferNum) {
                if (!getline(farray[j], data[j][count]))//从当前文件中读一行放到对应文件最小数据的数组中
                    ///如果当前文件已经读完了，也就是说是一个空文件，那么farray[i].eof()应该为true
                {
                    hasnoNext[j] = true;
                    break;
                    //cout<<"hasnoNext["<<i<<"]="<<hasnoNext[i]<<endl;
                }//标记该文件是空的
                count++;
                //  cout<<"读出文件"<<i<<"中数据放入 data["<<i<<"]="<<data[i]<<endl;
            }
            capacity[j] = count;
            cursor[j] = 0;
        }
    }
    while(cursor[j]<capacity[j]){
        fout<<data[j][cursor[j]++]<<endl;
    }
    while(getline(farray[j],data[j][0])){//最后剩下的那个文件的所有剩余行都保存到输出文件里去。
        fout<<data[j][0]<<endl;
        // cout<<"剩余最后一个文件"<<j<<"元素："<<data[j]<<"放入结果！"<<endl;
    }
    for(int k =0;k<in_file_sum;k++){
       delete [] data[k];
        farray[k].close();
    }
    delete [] data;
    delete [] hasnoNext;
    delete [] farray;
    fout.close();
}
void merge_sort(int in_file_sum,string in_dir,string out_file){
    if(in_file_sum<=1) return;
    //归并临时文件
    ifstream  * farray = new ifstream[in_file_sum];//打开多文件输入的句柄
    bool *hasnoNext = new bool[in_file_sum];//标记文件是否读完
    ofstream fout;
    fout.open(out_file.c_str(),ios::app|ios::ate);
    string * data = new string[in_file_sum];
    for(int i=0;i<in_file_sum;i++){
        farray[i].open((in_dir+inttostring(i)).c_str(),ios::in);//为每一个文件指针打开文件

        if(!farray[i].is_open()){
            cout<<"文件"<<i<<"没有正确打开！"<<endl;
            exit(0);
        }
        if(!getline(farray[i],data[i]))//从当前文件中读一行放到对应文件最小数据的数组中
            ///如果当前文件已经读完了，也就是说是一个空文件，那么farray[i].eof()应该为true
        {hasnoNext[i]=true;
            //cout<<"hasnoNext["<<i<<"]="<<hasnoNext[i]<<endl;
        }//标记该文件是空的
        //  cout<<"读出文件"<<i<<"中数据放入 data["<<i<<"]="<<data[i]<<endl;

    }
    int j=0;//用来记录最后排序剩下的那个文件的下标。
    while(true){
        string min;
        bool stop = false;

        for(int i=0;i<in_file_sum;i++){//找到第一个非空的假设最小值
            if(!hasnoNext[i])//没有下一个，也就是在getline时
            {min = data[i];
                j= i;
                break;
            }
        }
        for(int i = j+1;i<in_file_sum;i++){//循环找出最小的元素，交给min，并记录到下标在j
            if(!hasnoNext[i]&&get_id_from_file(data[i]).compare(get_id_from_file(min))<0){
                min = data[i];
                j =i;
            }
            if(!hasnoNext[i]) stop =true;
        }

        fout<<min<<endl;
        //cout<<"从文件"<<j<<"找出最小的元素放入文件："<<min<<endl;
        fout.flush();
        if(!stop) {
            // cout<<"跳出循环，终止排序"<<endl;
            break;
        }
        if(!getline(farray[j],data[j]))//从当前文件中读一行放到对应文件最小数据的数组中
            ///如果当前文件已经读完了，也就是说是一个空文件，那么farray[i].eof()应该为true
            hasnoNext[j]=true;//标记该文件是空的
    }
    while(getline(farray[j],data[j])){//最后剩下的那个文件的所有剩余行都保存到输出文件里去。
        fout<<data[j]<<endl;
        // cout<<"剩余最后一个文件"<<j<<"元素："<<data[j]<<"放入结果！"<<endl;
    }
    delete [] data;
    delete [] hasnoNext;
    for(int i=0;i<in_file_sum;i++){
        farray[i].close();
    }
    delete [] farray;
    fout.close();
}
void get_score(string in_file,map<string,double> & pagescore,int N,double a){
    ifstream in;
    in.open(in_file.c_str(),ios::in);
    string line;
    int index ;
    int id;
    double score;
    getline(in,line);
    while(!in.eof()){
        double sum=0.0;
        index = line.find_first_of("\t",0);
        string id  = line.substr(0,index);//
        score  = atof(line.substr(index+1).c_str());
        sum+=score;
        while(getline(in,line)&&get_id_from_file(line).compare(id)==0){
            sum+=get_score(line);
        }
        //cout<<id<<"\t"<<"sum="<<sum<<endl;
        //cout<<id<<"\t"<<"sum="<<sum<<endl;//////////计算出分数，插入内存结果map中
        double score = a*sum+(1.0-a)/N;
        pagescore.insert(pair<string,double>(id,score));
    }
    in.close();
}

#endif //SORT_GROUPING_MERGESORT_H
