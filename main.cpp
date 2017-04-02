#include <iostream>
#include <string>
#include <fstream>
#include "mergesort.h"
#include "quicksort.h"
#include "timer.h"
using namespace std;
int QuickSortNum =20;
int EveFileMergeBufferNum = 5;
void sort_grouping(string fname, string dir,string out) {/////从路径fname中读取源文件，然后生成消息写到pg_message
    ifstream source_in;
    ofstream source_out;
    string line;
    source_in.open(fname.c_str(),ios::in);
    if(!source_in){
        //cout<<"/home/kxw/data/pagerank/pg_message 无法打开！"<<endl;
        cout<<fname<<"无法打开！"<<endl;
        //cout<<"/home/kxw/ssddata/pagerank/pg_message 无法打开！"<<endl;
        exit(0);
    }
    getline(source_in,line);//  cout<<"从pg_message读出一行："<<line;
    int one_quicksort_count = 0;

    int outlabel = 0;
    kv  * page_id_score = new kv[QuickSortNum];
    //delete [] page_score;
    while(!source_in.eof()){
        one_quicksort_count++;
        int   index = line.find_first_of("\t",0);
        page_id_score[one_quicksort_count-1].k = line.substr(0,index);//将分数转成double型存在要排序的数组里
        page_id_score[one_quicksort_count-1].v = line.substr(index);
        if(one_quicksort_count==QuickSortNum){
            QuickSort(page_id_score,0,QuickSortNum-1);//当前排序池满，调用快速排序进行排序
            source_out.open((dir+inttostring(outlabel)).c_str(),ios::out);
            //source_out.open(("/home/kxw/data/pagerank/"+inttostring(outlabel)).c_str(),ios::out);
            // source_out.open(("/home/kxw/ssddata/pagerank/"+inttostring(outlabel)).c_str(),ios::out);
            if(!source_out.is_open())   cout<<"0,1,2...文件打不开！"<<endl;
            for(int i=0;i<QuickSortNum;i++){
                source_out<<page_id_score[i].k+"\t"+page_id_score[i].v<<endl;
                source_out.flush();
            }
            source_out.close();
            one_quicksort_count = 0;
            outlabel++;
        }
        getline(source_in,line);
        //     cout<<"从pg_message读出一行："<<line;
    }
    source_in.close();
    if(one_quicksort_count>0){//最后那一个排序池没填满，仍需要排序
        //  cout<<"进来了要排序"<<endl;
        QuickSort(page_id_score,0, one_quicksort_count-1);//当前排序池满，调用快速排序进行排序
        source_out.open((dir+inttostring(outlabel)).c_str(),ios::out);
        //source_out.open(("/home/kxw/data/pagerank/"+inttostring(outlabel)).c_str(),ios::out);
        // source_out.open(("/home/kxw/ssddata/pagerank/"+inttostring(outlabel)).c_str(),ios::out);
        if(!source_out.is_open())   cout<<outlabel<<"号文件打不开！"<<endl;
        for(int i=0;i<one_quicksort_count;i++){
            source_out<<page_id_score[i].k+"\t"+page_id_score[i].v<<endl;
            source_out.flush();
        }
        source_out.close();
        outlabel++;//一共有几个快排文件，也就意味着下一次合并排序的文件个数。
    }
    if(outlabel==1){
        //get_score(dir+"/0",pagescore,count,a);
        cout<<"只有一个文件，无需merge排序，直接生成分组！"<<endl;
    }
    else {///调用带缓存的合并排序函数
        merge_sort_buffer(outlabel, dir, out,EveFileMergeBufferNum);
        cout<<"混合排序完成！"<<endl;
    }
    delete [] page_id_score;

    for(int k =0;k<outlabel;k++){
       if(remove((dir+inttostring(k)).c_str()))  cout<<"删除文件pagerank/0,1,2...失败！"<<endl;
    }
    if(remove(out.c_str()))  cout<<"删除文件merge_res失败！"<<endl;
}
int main(int argc, char** argv) {
    /*参数列表：
    (1)dir(基本路径，结果输出的路径)
    (2)in(输入文件路径)
    (3)QuickSortNum(缓存大小) 单位万
    (4)EveFileMergeBufferNum(合并排序时每个文件缓存大小) 单位万
     */
    ////下面要做的是进行混合排序，先快速排序成多个子文件，再merge排序。

    ///////////////////基础路径和权重数据结构定义//////////////////////////////
    // string dir="/home/kxw/ssddata/pagerank/";                      ///
    // string out="/home/kxw/ssddata/pagerank/merge_res";             ///
    //char res[256] = "/home/kxw/ssddata/pagerank/pg_res";          ///
    // string dir="/home/kxw/data/pagerank/";                      ///
    // string out="/home/kxw/data/pagerank/merge_res";             ///
    //char res[256] = "/home/kxw/data/pagerank/pg_res";          ///

    string in ="/home/kxw/研一开题/data/pagerank/pg_message";
    string dir="/home/kxw/研一开题/data/pagerank/";
    string out=dir+"merge_res";
    Timer timer;
    if(argc<5) {
        cout<<"参数个数过少！使用默认参数咯！"<<endl;
        sort_grouping(in,dir,out);
    }else {
        cout<<"参数个数匹配！"<<endl;
        dir = argv[1];
        in = argv[2];
        out = dir+"merge_res";
        QuickSortNum = atoi(argv[3])*10000;
        EveFileMergeBufferNum = atoi(argv[4])*10000;
        sort_grouping(in, dir, out);
    }
    cout<<timer.elapsed()<<endl;
    ///上面要做的是进行混合排序，先快速排序成多个子文件，再merge排序。
   /*
    * 测试数据集，当前函数是将key都作为string类型来进行排序的！
    * string in ="/home/kxw/研一开题/data/pagerank/sort_grouping_kv";
    string dir="/home/kxw/研一开题/data/pagerank/";
    string out=dir+"merge_res";
    Timer timer;
    if(argc<4) {
        cout<<"参数个数过少！使用默认参数咯！"<<endl;
        sort_grouping(in,dir,out);
    }else {
        cout<<"参数个数匹配！"<<endl;
        dir = argv[1];
        in = argv[2];
        QuickSortNum = atoi(argv[3])*10000;
        sort_grouping(in, dir, out);
    }
    cout<<timer.elapsed()<<endl;*/
    return 0;
}