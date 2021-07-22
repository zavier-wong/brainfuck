#include <string>
#include <map>
#include <stack>
#include <cstring>
#include <cstdlib>
#include <iostream>
constexpr int CodeSegmentSize=10000;
constexpr int StackSegmentSize=10000;

class VirtualMachine{
public:
    VirtualMachine(){
        memset(CodeSegment,0,CodeSegmentSize);
        memset(StackSegment,0,StackSegmentSize);
    }
    void run(){
        while(codeTop()&&CodePointer<CodeSegmentSize){
            operate(codeTop());
            CodePointer++;
        }
    }
    void load(const std::string &input) {
        const char *tokens="+-><[],.";
        int idx=0;
        for(auto c:input){
            if(strchr(tokens,c)){
                if(idx==CodeSegmentSize)
                    machineError("代码过长");
                if(c=='['){
                    MatchBracket.push(idx);
                }else if(c==']'){
                    if(MatchBracket.empty())
                        machineError("无法匹配的]");
                    MapBracket[idx]=MatchBracket.top();
                    MapBracket[MatchBracket.top()]=idx;
                    MatchBracket.pop();
                }
                CodeSegment[idx++]=c;
            }
        }
        if(MatchBracket.size())
            machineError("无法匹配的[");
    }
private:
    char CodeSegment[CodeSegmentSize];
    char StackSegment[StackSegmentSize];

    int CodePointer=0;
    int StackPointer=0;

    char &codeTop(){
        return CodeSegment[CodePointer];
    }
    char &StackTop(){
        return StackSegment[StackPointer];
    }
    void machineError(const std::string &msg){
        puts(msg.c_str());
        exit(1);
    };
    std::stack<int> MatchBracket;
    std::map<int,int> MapBracket;
    void forward(){
        if(StackPointer==StackSegmentSize)
            machineError("Stack Overflow");
        StackPointer++;
    }
    void backward(){
        if(StackPointer==0)
            machineError("Segment Fault");
        StackPointer--;
    }
    void increment(){
        StackTop()++;
    }
    void decrement(){
        StackTop()--;
    }
    void input(){
        int c=getchar();
        StackTop()=c;
    }
    void output(){
        int c=StackTop();
        putchar(c);
    }
    void entryWhile(){
        if(!StackTop()){
            CodePointer=MapBracket[CodePointer];
        }
    }
    void exitWhile(){
        if(StackTop()){
            CodePointer=MapBracket[CodePointer]-1;
        }
    }
    void operate(char c){
        switch (c) {
            case '+': increment();break;
            case '-': decrement();break;
            case '>': forward();break;
            case '<': backward();break;
            case '[': entryWhile();break;
            case ']': exitWhile();break;
            case ',': input();break;
            case '.': output();break;
        }
    }
};

int main(int argc,char *argv[]) {
    VirtualMachine VM;
    if(argc>1){
        freopen(argv[1],"r",stdin);
        std::string code,tmp;
        while(getline(std::cin,tmp))
            code+=tmp;
        VM.load(code);
        VM.run();
    }
    std::cout<<std::endl;
    return 0;
}
