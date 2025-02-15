#include <bits/stdc++.h>
#include <unistd.h>
#define underline "\033[4m"
#define closeunderline "\033[24m"
// #define bold "\e[1m"
// #define closebold "\e[0m"
#define MemSize 32
using namespace std;
class Assembler
{
    public:
    class FuncTypes
    {
        public:
        map <string,int> R,I,L,S,B,J,U;
    };
    void printVecStr(vector<string> Line)
    {
        for(int i=0;i<Line.size();i++)
        {
            cout<<Line[i]<<" ";
        }
        cout<<endl;
    }
    bool isBreak(char ch)
    {
        if(ch==' '||ch==',')
            return true;
        return false;
    }
    bool isLineEnd(char ch)
    {
        if(ch==';')
            return true;
        return false;
    }
    void toUpperCase(string &s)
    {
        int l=s.length();
        for(int i=0;i<l;i++)
        {
            if(s[i]>='a'&&s[i]<='z')
                s[i]+='A'-'a';
        }
    }
    string DectoBin(int n)
    {
        string bin;
        while(n>0)
        {
            bin+=('0'+(n%2));
            n/=2;
        }
        reverse(bin.begin(),bin.end());
        return bin;
    }
    string HextoBin(char ch)
    {
        switch(ch)
        {
            case '0':
                return "0000";
            case '1':
                return "0001";
            case '2':
                return "0010";
            case '3':
                return "0011";
            case '4':
                return "0100";
            case '5':
                return "0101";
            case '6':
                return "0110";
            case '7':
                return "0111";
            case '8':
                return "1000";
            case '9':
                return "1001";
            case 'A':
                return "1010";
            case 'B':
                return "1011";
            case 'C':
                return "1100";
            case 'D':
                return "1101";
            case 'E':
                return "1110";
            case 'F':
                return "1111";
            default:
                return "";
        }
    }
    int StoI(string s)
    {
        int ans=0;
        int n=s.size();
        for(int i=0;i<n;i++)
        {
            if(s[i]>='0'&&s[i]<='9')
            {
                ans*=10;
                ans+=s[i]-'0';
            }
        }
        return ans;
    }
    void extend0(string &s,int n)
    {
        int l=s.size();
        if(l!=n)
        {
            string temp="";
            for(int i=1;i<=n-l;i++)
                temp+='0';
            s=temp+s;
        }
    }
    void extend1(string &s,int n)
    {
        int l=s.size();
        if(l!=n)
        {
            string temp="";
            for(int i=1;i<=n-l;i++)
                temp+='1';
            s=temp+s;
        }
    }
    void truncate(string &s,int n)
    {
        int l=s.size();
        if(l>n)
            s=s.substr(0,n);
    }
    string ImmGen(string Imm,int n)
    {
        toUpperCase(Imm);
        string ans="";
        if(Imm[1]=='B'&&Imm[0]=='0')    //binary
        {
            for(int i=2;i<Imm.length();i++)
            {
                if(Imm[i]=='0'||Imm[i]=='1')
                    ans+=Imm[i];
            }
            if(ans[0]!='1')
                extend0(ans,n);
            else
                extend1(ans,n);
            truncate(ans,n);
        }
        else if(Imm[1]=='X'&&Imm[0]=='0')   //hexadecimal
        {
            for(int i=2;i<Imm.length();i++)
            {
                ans+=HextoBin(Imm[i]);
            }
            if(ans[0]!='1')
                extend0(ans,n);
            else
                extend1(ans,n);
            truncate(ans,n);
        }
        else    //decimal
        {
            int imm=StoI(Imm);  
            ans=DectoBin(imm);
            truncate(ans,n-1);
            extend0(ans,n);
            if(Imm[0]=='-') //taking 2s complement
            {
                bool flag1=false;
                for(int i=n-1;i>=0;i--)
                {
                    if(flag1)
                        ans[i]='0'+'1'-ans[i];
                    else if(ans[i]=='1')
                        flag1=true;
                }
            }
        }
        return ans;
    }
    int RegGen(string &Reg)
    {
        toUpperCase(Reg);
        if(!(Reg[0]=='R'||Reg[0]=='X'))
            return -1;
        Reg=Reg.substr(1,Reg.size()-1);
        int reg=StoI(Reg);
        if(reg>31)
            return 1;
        Reg=DectoBin(reg);
        extend0(Reg,5);
        return 0;
    }
    void makeMaps(FuncTypes &F)
    {
        //Register
        F.R["ADD"]=0;
        F.R["SLL"]=1;
        F.R["SLT"]=2;
        F.R["SLTU"]=3;
        F.R["XOR"]=4;
        F.R["SRL"]=5;
        F.R["OR"]=6;
        F.R["AND"]=7;
        F.R["SUB"]=10;
        F.R["MUL"]=11;
        F.R["DIV"]=12;
        F.R["REM"]=13;
        F.R["SRA"]=15;
        //Immediate
        F.I["ADDI"]=0;
        F.I["SLTI"]=2;
        F.I["SLTIU"]=3;
        F.I["XORI"]=4;
        F.I["ORI"]=6;
        F.I["ANDI"]=7;
        F.I["SLLI"]=21;
        F.I["SRLI"]=25;
        F.I["SRAI"]=15;
        //Load
        F.L["LB"]=0;
        F.L["LH"]=1;
        F.L["LW"]=2;
        F.L["LBU"]=4;
        F.L["LHU"]=5;
        //Store
        F.S["SB"]=0;
        F.S["SH"]=1;
        F.S["SW"]=2;
        //Branch
        F.B["BEQ"]=0;
        F.B["BNE"]=1;
        F.B["BLT"]=4;
        F.B["BGE"]=5;
        F.B["BLTU"]=6;
        F.B["BGEU"]=7;
        //Jump and Link
        F.J["JAL"]=-1;
        F.J["JALR"]=0;
        //Upper Immediate
        F.U["LUI"]=-1;
        F.U["AUIPC"]=-2;
    }
    string ParseR(vector<string> assl,FuncTypes F)
    {
        int n=assl.size();
        string ans="";
        if(n==4)
        {
            string opcode="0110011";
            string func3=DectoBin((F.R[assl[0]])%10);
            extend0(func3,3);
            string func7=((F.R[assl[0]])/10)?"0100000":"0000000";
            string Rd=assl[1],Rs1=assl[2],Rs2=assl[3];
            int rd=RegGen(Rd),rs1=RegGen(Rs1),rs2=RegGen(Rs2);
            if(rd==-1||rs1==-1||rs2==-1)
                cout<<"\nExpected Register Number\n";
            else if(rd==1||rs1==1||rs2==1)
                cout<<"\nInvalid Register Number\n";
            else
                ans=func7+Rs2+Rs1+func3+Rd+opcode;
        }
        else
        {
            cout<<"\nInvalid number of arguments in R\n";
        }
        return ans;
    }
    string ParseI(vector<string> assl,FuncTypes F)
    {
        int n=assl.size();
        string ans="";
        if(n==4)
        {
            string opcode="0010011";
            string func3=DectoBin((F.I[assl[0]])%10);
            extend0(func3,3);
            string Imm;
            string Rd=assl[1],Rs=assl[2];
            int rd=RegGen(Rd),rs=RegGen(Rs);
            if(rd==-1||rs==-1)
                cout<<"\nExpected Register Number\n";
            else if(rd==1||rs==1)
                cout<<"\nInvalid Register Number\n";
            else
            {
                if(F.I[assl[0]]/10==0)
                {
                    Imm=ImmGen(assl[3],12);
                }
                else
                {
                    string func7=((F.I[assl[0]])/10==1)?"0100000":"0000000";
                    string shamt5=ImmGen(assl[3],5);
                    Imm=func7+shamt5;
                }
                ans=Imm+Rs+func3+Rd+opcode;
            }
        }
        else
        {
            cout<<"\nInvalid number of arguments in I\n";
        }
        return ans;
    }
    string ParseL(vector<string> assl,FuncTypes F)
    {
        int n=assl.size();
        string ans="";
        if(n==3)
        {
            string opcode="0000011";
            string func3=DectoBin((F.L[assl[0]])%10);
            extend0(func3,3);
            string Rd=assl[1];
            string Imm12="",Rs="";
            bool open=false,close=false;
            for(int i=0;i<assl[2].size();i++)
            {
                if(assl[2][i]=='[')
                {
                    open=true;
                    continue;
                }
                else if(assl[2][i]==']')
                {
                    close=true;
                    break;
                }
                if(open)
                    Rs+=assl[2][i];
                else
                    Imm12+=assl[2][i];
            }
            if(!open&&!close)
            {
                Rs=Imm12;
                Imm12="";
                open=close=true;
            }
            if(open&&close)
            {
                int rd=RegGen(Rd),rs=RegGen(Rs);
                if(rd==-1||rs==-1)
                    cout<<"\nExpected Register Number\n";
                else if(rd==1||rs==1)
                    cout<<"\nInvalid Register Number\n";
                else
                {
                    Imm12=ImmGen(Imm12,12);
                    ans=Imm12+Rs+func3+Rd+opcode;
                }
            }
            else
                cout<<"\nInvalid Source Format in L\n";
        }
        else
        {
            cout<<"\nInvalid number of arguments in L\n";
        }
        return ans;
    }
    string ParseS(vector<string> assl,FuncTypes F)
    {
        int n=assl.size();
        string ans="";
        if(n==3)
        {
            string opcode="0100011";
            string func3=DectoBin((F.S[assl[0]])%10);
            extend0(func3,3);
            string Rsrc=assl[1];
            string Imm12="",Rbase="";
            bool open=false,close=false;
            for(int i=0;i<assl[2].size();i++)
            {
                if(assl[2][i]=='[')
                {
                    open=true;
                    continue;
                }
                else if(assl[2][i]==']')
                {
                    close=true;
                    break;
                }
                if(open)
                    Rbase+=assl[2][i];
                else
                    Imm12+=assl[2][i];
            }
            if(!open&&!close)
            {
                Rbase=Imm12;
                Imm12="";
                open=close=true;
            }
            if(open&&close)
            {
                int rsrc=RegGen(Rsrc),rbase=RegGen(Rbase);
                if(rsrc==-1||rbase==-1)
                    cout<<"\nExpected Register Number\n";
                else if(rsrc==1||rbase==1)
                    cout<<"\nInvalid Register Number\n";
                else
                {
                    Imm12=ImmGen(Imm12,12);
                    string imm7=Imm12.substr(0,7);
                    string imm5=Imm12.substr(7,5);
                    ans=imm7+Rsrc+Rbase+func3+imm5+opcode;
                }
            }
            else
                cout<<"\nInvalid Source Format in S\n";
        }
        else
        {
            cout<<"\nInvalid number of arguments in S\n";
        }
        return ans;
    }
    string ParseB(vector<string> assl,FuncTypes F)
    {
        int n=assl.size();
        string ans="";
        if(n==4)
        {
            string opcode="1100011";
            string func3=DectoBin((F.B[assl[0]])%10);
            extend0(func3,3);
            string Rs1=assl[1],Rs2=assl[2],Imm12=assl[3];
            int rs1=RegGen(Rs1),rs2=RegGen(Rs2);
            if(rs1==-1||rs2==-1)
                cout<<"\nExpected Register Number\n";
            else if(rs1==1||rs2==1)
                cout<<"\nInvalid Register Number\n";
            else
            {
                Imm12=ImmGen(Imm12,12);
                string imm7=Imm12.substr(0,7);
                string imm5=Imm12.substr(7,5);
                ans=imm7+Rs2+Rs1+func3+imm5+opcode;
            }
        }
        else
        {
            cout<<"\nInvalid number of arguments in B\n";
        }
        return ans;
    }
    string ParseJ(vector<string> assl,FuncTypes F)
    {
        int n=assl.size();
        string ans="";
        if(n==3)
        {
            if(F.J[assl[0]]==-1)
            {
                string opcode="1101111";
                string Rd=assl[1];
                string Imm20=assl[2];
                int rd=RegGen(Rd);
                if(rd==-1)
                 cout<<"\nExpected Register Number\n";
                else if(rd==1)
                    cout<<"\nInvalid Register Number\n";
                else
                {
                    Imm20=ImmGen(Imm20,20);
                    ans=Imm20+Rd+opcode;
                }
            }
            else
            {
                string opcode="1100111";
                string func3="000";
                string Rd=assl[1];
                string Imm12="",Rs="";
                bool open=false,close=false;
                for(int i=0;i<assl[2].size();i++)
                {
                    if(assl[2][i]=='[')
                    {
                        open=true;
                        continue;
                    }
                    else if(assl[2][i]==']')
                    {
                        close=true;
                        break;
                    }
                    if(open)
                        Rs+=assl[2][i];
                    else
                        Imm12+=assl[2][i];
                }
                if(!open&&!close)
                {
                    Rs=Imm12;
                    Imm12="";
                    open=close=true;
                }
                if(open&&close)
                {
                    int rd=RegGen(Rd),rs=RegGen(Rs);
                    if(rd==-1||rs==-1)
                        cout<<"\nExpected Register Number\n";
                    else if(rd==1||rs==1)
                        cout<<"\nInvalid Register Number\n";
                    else
                    {
                        Imm12=ImmGen(Imm12,12);
                        ans=Imm12+Rs+func3+Rd+opcode;
                    }
                }
                else
                    cout<<"\nInvalid Source Format in JALR\n";
            }
        }
        else
        {
            cout<<"\nInvalid number of arguments in JAL/JALR/n";
        }
        return ans;
    }
    string ParseU(vector<string> assl,FuncTypes F)
    {
        int n=assl.size();
        string ans="";
        if(n==3)
        {
            string opcode=(F.U[assl[0]]==-1)?"0110111":"0010111";
            string Imm20=assl[2];
            string Rd=assl[1];
            int rd=RegGen(Rd);
            if(rd==-1)
             cout<<"\nExpected Register Number\n";
            else if(rd==1)
                cout<<"\nInvalid Register Number\n";
            else
            {
                Imm20=ImmGen(Imm20,20);
                ans=Imm20+Rd+opcode;
            }
        }
        else
        {
            cout<<"\nInvalid number of arguments in U\n";
        }
        return ans;
    }
    string ParseLine(vector <string> assl,FuncTypes F)
    {
        string b;
        int n=assl.size();
        if(n==0)
            return b;
        toUpperCase(assl[0]);
        if(F.R.find(assl[0])!=F.R.end())
        {
            b=ParseR(assl,F);
        }
        else if(F.I.find(assl[0])!=F.I.end())
        {
            b=ParseI(assl,F);
        }
        else if(F.L.find(assl[0])!=F.L.end())
        {
            b=ParseL(assl,F);
        }
        else if(F.S.find(assl[0])!=F.S.end())
        {
            b=ParseS(assl,F);
        }
        else if(F.B.find(assl[0])!=F.B.end())
        {
            b=ParseB(assl,F);
        }
        else if(F.J.find(assl[0])!=F.J.end())
        {
            b=ParseJ(assl,F);
        }
        else if(F.U.find(assl[0])!=F.U.end())
        {
            b=ParseU(assl,F);
        }
        else
        {
            cout<<"\n"<<assl[0]<<" is not a valid function\n";
        }
        return b;
    }
    void Parse(string ass,string &bin,FuncTypes F)
    {
        int l=ass.length();
        string token="";
        vector<string> tokenline;
        cout<<underline<<"\nTokens:"<<closeunderline<<endl;
        for(int i=0;i<l;i++)
        {
            if(isBreak(ass[i]))
            {
                if(token!="")
                    tokenline.push_back(token);
                token="";
            }
            else if(isLineEnd(ass[i]))
            {
                if(token!="")
                    tokenline.push_back(token);
                bin+=ParseLine(tokenline,F)+"\n";
                printVecStr(tokenline);
                tokenline.clear();
                token="";
            }
            else
            {
                token+=ass[i];
            }
        }
    }
    string main()
    {
        string ass,bin;
        cout<<"1. All statements must be written in one line.\n2. Use ';' to end a statement."<<endl;
        cout<<underline<<"Enter Assembly:"<<closeunderline<<endl;
        getline(cin,ass);
        FuncTypes F;
        makeMaps(F);
        Parse(ass,bin,F);
        return bin;
    }
};
class PipeLineStall{
    public:
    class ControlWord
    {
        public:
        bool AluSrc,Mem2Reg,RegWrite,MemRead,MemWrite,Branch,Jump;
        char AluOp;
        bool Ready;
        ControlWord()
        {
            AluSrc=Mem2Reg=RegWrite=MemRead=MemWrite=Branch=Jump=false;
            AluOp='\0';
            Ready=true;
        }
    };
    class FirstStage
    {
        public:
        int pc;
        bool valid;
        FirstStage()
        {
            pc=0;
            valid=false;
        }
    };
    class SecondStage
    {
        public:
        int DPC;
        string IR;
        bool valid,stall;
        SecondStage()
        {
            DPC=0;
            IR="";
            valid=stall=false;
        }
    };
    class ThirdStage
    {
        public:
        ControlWord CW;
        int DPC,JPC;
        string IMMbr,RS1,RS2,IMMalu,Func,Rdl;
        string Rs1,Rs2;
        bool valid;
        ThirdStage()
        {
            DPC=JPC=0;
            IMMbr=RS1=RS2=IMMalu=Func=Rdl="";
            Rs1=Rs2="";
            valid=false;
        }
    };
    class FourthStage
    {
        public:
        ControlWord CW;
        string ALUOUT,RS2,Rdl;
        char DataControl;
        bool valid;
        FourthStage()
        {
            ALUOUT=RS2=Rdl="";
            DataControl='\0';
            valid=false;
        }
    };
    class FifthStage
    {
        public:
        ControlWord CW;
        string LDOUT,ALUOUT,Rdl;
        bool valid;
        FifthStage()
        {
            LDOUT=ALUOUT=Rdl="";
            valid=false;
        }
    };
    string SignedExtend32(string bin)
    {
        string ans=bin;
        if(ans.length()<=1)
            ans="0"+ans;
        while(ans.length()<32)
            ans=ans[0]+ans;
        if(ans.length()>32)
            ans=ans.substr(0,32);
        return ans;
    }
    string ZeroExtend32(string bin)
    {
        string ans=bin;
        while(ans.length()<32)
            ans=ans+"0";
        if(ans.length()>32)
            ans=ans.substr(0,32);
        return ans;
    }
    long BinToDec(string bin)
    {
        if(bin.length()==0)
            return 0;
        long dec=0;
        int n=bin.length();
        long pow2=1;
        for(int i=n-1;i>0;i--)
        {
            if(bin[i]=='1')
                dec+=pow2;
            pow2*=2;
        }
        if(bin[0]=='1')
            dec-=pow2;
        return dec;
    }
    string DecToBin(long dec)
    {
        string bin="";
        long n=abs(dec);
        while(n>0)
        {
            bin+=('0'+(n%2));
            n/=2;
        }
        reverse(bin.begin(),bin.end());
        bin="0"+bin;
        if(dec<0)   //for 2s complement
        {
            n=bin.length();
            bool flag1=false;
            for(int i=n-1;i>=0;i--)
            {
                if(flag1)
                    bin[i]='0'+'1'-bin[i];
                else if(bin[i]=='1')
                    flag1=true;
            }
        }
        bin=SignedExtend32(bin);
        return bin;
    }
    FirstStage PC;
    SecondStage IFID;
    ThirdStage IDEX;
    FourthStage EXMO;
    FifthStage MOWB;
    vector<string> InstMem;
    vector<string> GPR;
    vector<bool> GPRReady;
    vector<string> DataMem;
    PipeLineStall()
    {
        InstMem=vector<string> (MemSize,"");
        DataMem=vector<string> (MemSize,"");
        GPR=vector<string> (32,SignedExtend32("0"));
        GPRReady=vector<bool> (32,true);
    }
    ControlWord ControlUnit(string opcode)
    {
        ControlWord CW;
        if(opcode=="0110011")   //R Type
        {
            CW.RegWrite=true;
            CW.AluOp='R';
            CW.Ready=false;
        }
        else if(opcode=="0010011")   //I Type
        {
            CW.AluSrc=true;
            CW.RegWrite=true;
            CW.AluOp='I';
            CW.Ready=false;
        }
        else if(opcode=="0000011")   //L Type
        {
            CW.AluSrc=true;
            CW.Mem2Reg=true;
            CW.RegWrite=true;
            CW.MemRead=true;
            CW.AluOp='L';
            CW.Ready=false;
        }
        else if(opcode=="0100011")   //S Type
        {
            CW.MemWrite=true;
            CW.AluOp='S';
        }
        else if(opcode=="1100011")  //B Type
        {
            CW.Branch=true;
            CW.AluOp='B';
        }
        else if(opcode=="1101111")   //JAL
        {
            CW.RegWrite=true;
            CW.Jump=true;
            CW.AluOp='J';
            CW.Ready=false;
        }
        else if(opcode=="1100111")  //JALR
        {
            CW.AluSrc=true;
            CW.RegWrite=true;
            CW.Jump=true;
            CW.AluOp='I';
            CW.Ready=false;
        }
        else if(opcode=="0110111")   //LUI
        {
            CW.AluSrc=true;
            CW.RegWrite=true;
            CW.AluOp='U';
            CW.Ready=false;
        }
        else if(opcode=="0010111")  //AUIPC
        {
            CW.AluSrc=true;
            CW.RegWrite=true;
            CW.AluOp='A';
            CW.Ready=false;
        }
        return CW;
    }
    char ALUControlUnit(string func3,char func7)
    {
        char AluOp=IDEX.CW.AluOp;
        char operation=';';
        if(AluOp=='R')
        {
            if(func7=='1')
            {
                if(func3=="000")
                    operation='-';
                else if(func3=="001")
                    operation='*';
                else if(func3=="010")
                    operation='/';
                else if(func3=="011")
                    operation='%';
                else if(func3=="101")
                    operation='r';
            }
            else
            {
                if(func3=="000")
                    operation='+';
                else if(func3=="001")
                    operation='<';
                else if(func3=="010")
                    operation='(';
                else if(func3=="011")
                    operation='[';
                else if(func3=="100")
                    operation='^';
                else if(func3=="101")
                    operation='>';
                else if(func3=="110")
                    operation='|';
                else if(func3=="111")
                    operation='&';
            }
        }
        else if(AluOp=='I')
        {
            if(func3=="000")
                operation='+';
            else if(func3=="010")
                operation='(';
            else if(func3=="011")
                operation='[';
            else if(func3=="100")
                operation='^';
            else if(func3=="110")
                operation='|';
            else if(func3=="111")
                operation='&';
            else if(func3=="001")
                if(func7=='0')
                    operation='<';
            else if(func3=="101")
                if(func7=='0')
                    operation='>';
                else
                    operation='r';
        }
        else if(AluOp=='L')
        {
            operation='+';
        }
        else if(AluOp=='S')
        {
            operation='#';
        }
        else if(AluOp=='B')
        {
            if(func3=="000")
                operation='=';
            else if(func3=="001")
                operation='!';
            else if(func3=="100")
                operation='<';
            else if(func3=="101")
                operation='>';
            else if(func3=="110")
                operation='(';
            else if(func3=="111")
                operation=')';
        }
        else if(AluOp=='U'||AluOp=='A')
        {
            operation='+';
        }
        return operation;
    }
    pair<bool,string> ALU(bool AluSrc,char AluOperation,bool isBranch)
    {
        long input1,input1U;
        long input2,input2U;
        input1=BinToDec(IDEX.RS1);
        input1U=BinToDec("0"+IDEX.RS1);
        if(AluSrc)
        {
            string Imm=IDEX.IMMalu;
            if(AluOperation=='<'||AluOperation=='>'||AluOperation=='r')
                Imm=Imm.substr(12-5,5);
            input2=BinToDec(Imm);
            input2U=BinToDec("0"+Imm);
        }
        else
        {
            string RS2=IDEX.RS2;
            if(AluOperation=='#')
                RS2=IDEX.IMMbr;
            input2=BinToDec(RS2);
            input2U=BinToDec("0"+RS2);
        }
        long output=0;
        bool branchFlag=false;
        long msb=input1-(input1%(1<<31));
        if(!isBranch)
        {
            switch(AluOperation)
            {
                case '+':
                    output=input1+input2;
                    break;
                case '#':
                    output=input1+input2;
                    break;
                case '-':
                    output=input1-input2;
                    break;
                case '*':
                    output=input1*input2;
                    break;
                case '/':
                    output=input1/input2;
                    break;
                case '%':
                    output=input1%input2;
                    break;
                case '&':
                    output=input1&input2;
                    break;
                case '|':
                    output=input1|input2;
                    break;
                case '^':
                    output=input1^input2;
                    break;
                case '<':
                    output=input1<<input2;
                    break;
                case '>':
                    output=input1>>input2;
                    break;
                case 'r':
                    output=input1;
                    while(input2)
                    {
                        input2--;
                        input1=msb+input1<<1;
                    }
                    break;
                case '(':
                    output=input1<input2;
                    break;
                case '[':
                    output=input1U<input2U;
                    break;
            }
        }
        else
        {
            switch(AluOperation)
            {
                case '=':
                    branchFlag=(input1==input2);
                    break;
                case '!':
                    branchFlag=(input1!=input2);
                    break;
                case '<':
                    branchFlag=(input1<input2);
                    break;
                case '>':
                    branchFlag=(input1>=input2);
                    break;
                case '(':
                    branchFlag=(input1U<input2U);
                    break;
                case ')':
                    branchFlag=(input1U>=input2U);
                    break;
            }
        }
        string Output=DecToBin(output);
        return {branchFlag,Output};
    }
    char DataSizeControlUnit(string func3)
    {
        char size='\0';
        if(func3=="000")
            size='B';
        else if(func3=="001")
            size='H';
        else if(func3=="010")
            size='W';
        else if(func3=="100")
            size='b';
        else if(func3=="101")
            size='h';
        return size;
    }
    string DataSize(string data,char size)
    {
        string ans=data;
        switch(size)
        {
            case 'B':
                ans=ans.substr(0,8);
                break;
            case 'b':
                ans="0"+ans.substr(0,8);
                break;
            case 'H':
                ans=ans.substr(0,16);
                break;
            case 'h':
                ans="0"+ans.substr(0,16);
                break;
            case 'W':
                ans=ans.substr(0,32);
                break;
        }
        ans=ZeroExtend32(ans);
        return ans;
    }
    void InstructionFetch()
    {
        if(IFID.stall)
            return;
        if(PC.valid)
        {
            if(PC.pc<0||PC.pc>=MemSize)
            {
                IFID.valid=false;
                PC.valid=false;
                return;
            }
            IFID.IR=InstMem[PC.pc];
            IFID.DPC=PC.pc;
            if(InstMem[PC.pc]=="")
            {
                IFID.valid=false;
                PC.valid=false;
            }
            else
                IFID.valid=true;
            PC.pc=PC.pc+1;
        }
    }
    void InstructionDecode()
    {
        GPR[0]=SignedExtend32("0"); //Hardwiring R0 to 0
        if(IFID.valid)
        {
            string imm20=(IFID.IR).substr(31-31,20);
            string func3=(IFID.IR).substr(31-14,3);
            string Rdl=(IFID.IR).substr(31-11,5);
            string controlword=(IFID.IR).substr(31-6,7);
            string Rs1=(IFID.IR).substr(31-19,5);
            string Rs2=(IFID.IR).substr(31-24,5);
            IDEX.Rs1=Rs1;
            IDEX.Rs2=Rs2;
            IDEX.Rdl=Rdl;
            IDEX.DPC=IFID.DPC;
            IDEX.JPC=IFID.DPC+BinToDec(imm20);
            IDEX.Func=func3;
            IDEX.CW=ControlUnit(controlword);
            int rs1=BinToDec("0"+Rs1);
            int rs2=BinToDec("0"+Rs2);
            if(IDEX.CW.AluOp=='U'||IDEX.CW.AluOp=='A')
                rs1=0;
            if(!GPRReady[rs1]||(!IDEX.CW.AluSrc&&!GPRReady[rs2]))
            {
                IDEX.valid=false;
                IFID.stall=true;
                return;
            }
            int rdl=BinToDec("0"+Rdl);
            GPRReady[rdl]=IDEX.CW.Ready;
            IDEX.RS1=GPR[rs1];
            IDEX.RS2=GPR[rs2];
            IDEX.IMMalu=(IFID.IR).substr(31-31,12);
            if(IDEX.CW.AluOp=='U')
            {
                IDEX.IMMalu=ZeroExtend32(imm20);
                IDEX.RS1=DecToBin(0);
                IDEX.Rs1="";
            }
            else if(IDEX.CW.AluOp=='A')
            {
                IDEX.IMMalu=ZeroExtend32(imm20);
                IDEX.RS1=DecToBin(IFID.DPC);
                IDEX.Rs1="";
            }
            IDEX.IMMbr=(IFID.IR).substr(31-31,7)+(IFID.IR).substr(31-11,5);
            IDEX.valid=true;
            IFID.stall=false;
        }
        else
        {
            IDEX.valid=false;
        }
        GPRReady[0]=true; //it is always 0
    }
    void InstructionExecute()
    {
        if(IDEX.valid)
        {
            char func30=IDEX.IMMalu[1];
            char AluSelect=ALUControlUnit(IDEX.Func,func30);
            pair <bool,string> receivefromAlu=ALU(IDEX.CW.AluSrc,AluSelect,IDEX.CW.Branch);
            string AluOut=receivefromAlu.second;
            bool branchFlag=receivefromAlu.first;
            EXMO.CW=IDEX.CW;
            EXMO.ALUOUT=AluOut;
            EXMO.RS2=IDEX.RS2;
            EXMO.Rdl=IDEX.Rdl;
            EXMO.DataControl=DataSizeControlUnit(IDEX.Func);
            EXMO.valid=true;
            int BPC=IDEX.DPC+BinToDec(IDEX.IMMbr);
            if(IDEX.CW.Branch&&branchFlag)
            {
                IFID.valid=false;
                EXMO.valid=false;
                PC.pc=BPC;
                PC.valid=true;
            }
            if(IDEX.CW.Jump)
            {
                IFID.valid=false;
                if(IDEX.CW.AluOp=='I')
                {
                    IDEX.JPC=BinToDec(AluOut);
                }
                EXMO.ALUOUT=DecToBin(IDEX.DPC+1);
                EXMO.valid=true;    //to write NPC in given register
                PC.pc=IDEX.JPC;
                PC.valid=true;
            }
        }
        else
        {
            EXMO.valid=false;
        }
    }
    void MemoryOperation()
    {
        if(EXMO.valid)
        {
            if(EXMO.CW.MemRead||EXMO.CW.MemWrite)
            {
                int effectiveAddress=BinToDec(EXMO.ALUOUT);
                if(effectiveAddress<0||effectiveAddress>=MemSize)
                {
                    cout<<"Invalid Address for Data Memory"<<endl;
                    return;
                }
                if(EXMO.CW.MemWrite)
                {
                    DataMem[effectiveAddress]=DataSize(EXMO.RS2,EXMO.DataControl);
                }
                if(EXMO.CW.MemRead)
                {
                    MOWB.LDOUT=DataSize(DataMem[effectiveAddress],EXMO.DataControl);
                }
            }
            MOWB.CW=EXMO.CW;
            MOWB.ALUOUT=EXMO.ALUOUT;
            MOWB.Rdl=EXMO.Rdl;
            if(EXMO.CW.RegWrite)
                MOWB.valid=true;
            else
                MOWB.valid=false;
        }
        else
        {
            MOWB.valid=false;
        }
    }
    void WriteBack()
    {
        if(MOWB.valid)
        {
            string writeData;
            if(MOWB.CW.Mem2Reg)
            {
                writeData=MOWB.LDOUT;
            }
            else
            {
                writeData=MOWB.ALUOUT;
            }
            if(MOWB.CW.RegWrite)
            {
                int rdl=BinToDec("0"+MOWB.Rdl);
                GPR[rdl]=writeData;
                GPRReady[rdl]=true;
            }
        }
    }
    void PrintMem()
    {
        int data=0;
        cout<<"General Purpose Registers:"<<endl;
        for(int i=0;i<32;i++)
        {
            data=BinToDec(GPR[i]);
            if(data!=0||i==0)
                cout<<"R"<<i<<": "<<data<<endl;
        }
        cout<<"\nData Memory:"<<endl;
        for(int i=0;i<MemSize;i++)
        {
            data=BinToDec(DataMem[i]);
            if(data!=0)
                cout<<"DM"<<i<<": "<<data<<endl;
        }
        cout<<endl;
    }
    void main(vector<string> &BinarySegmented)
    {
        cout<<underline<<"Instruction Memory:"<<closeunderline<<endl;
        for(int i=0;i<MemSize;i++)
        {
            InstMem[i]=BinarySegmented[i];
            if(InstMem[i]!="")
                cout<<i<<". "<<InstMem[i]<<endl;
        }
        cout<<endl<<underline<<"Simulating CPU:"<<closeunderline<<endl;
        bool validity=PC.valid=true;
        int cycle=1;
        while(validity)
        {
            sleep(1);
            cout<<underline<<"Cycle "<<cycle++<<":"<<closeunderline<<endl;
            WriteBack();
            MemoryOperation();
            InstructionExecute();
            InstructionDecode();
            InstructionFetch();
            PrintMem();
            validity=PC.valid||IFID.valid||IDEX.valid||EXMO.valid||MOWB.valid;
        }
    }
};
class PipeLineOperandForwarding{
    public:
    class ControlWord
    {
        public:
        bool AluSrc,Mem2Reg,RegWrite,MemRead,MemWrite,Branch,Jump;
        char AluOp;
        ControlWord()
        {
            AluSrc=Mem2Reg=RegWrite=MemRead=MemWrite=Branch=Jump=false;
            AluOp='\0';
        }
    };
    class FirstStage
    {
        public:
        int pc;
        bool valid;
        FirstStage()
        {
            pc=0;
            valid=false;
        }
    };
    class SecondStage
    {
        public:
        int DPC;
        string IR;
        bool valid,stall;
        SecondStage()
        {
            DPC=0;
            IR="";
            valid=stall=false;
        }
    };
    class ThirdStage
    {
        public:
        ControlWord CW;
        int DPC,JPC;
        string IMMbr,RS1,RS2,IMMalu,Func,Rdl;
        string Rs1,Rs2;
        bool valid,stall;
        ThirdStage()
        {
            DPC=JPC=0;
            IMMbr=RS1=RS2=IMMalu=Func=Rdl="";
            Rs1=Rs2="";
            valid=stall=false;
        }
    };
    class FourthStage
    {
        public:
        ControlWord CW;
        string ALUOUT,RS2,Rdl;
        char DataControl;
        bool valid,stall;
        FourthStage()
        {
            ALUOUT=RS2=Rdl="";
            DataControl='\0';
            valid=stall=false;
        }
    };
    class FifthStage
    {
        public:
        ControlWord CW;
        string LDOUT,ALUOUT,Rdl;
        bool valid;
        FifthStage()
        {
            LDOUT=ALUOUT=Rdl="";
            valid=false;
        }
    };
    string SignedExtend32(string bin)
    {
        string ans=bin;
        if(ans.length()<=1)
            ans="0"+ans;
        while(ans.length()<32)
            ans=ans[0]+ans;
        if(ans.length()>32)
            ans=ans.substr(0,32);
        return ans;
    }
    string ZeroExtend32(string bin)
    {
        string ans=bin;
        while(ans.length()<32)
            ans=ans+"0";
        if(ans.length()>32)
            ans=ans.substr(0,32);
        return ans;
    }
    long BinToDec(string bin)
    {
        if(bin.length()==0)
            return 0;
        long dec=0;
        int n=bin.length();
        long pow2=1;
        for(int i=n-1;i>0;i--)
        {
            if(bin[i]=='1')
                dec+=pow2;
            pow2*=2;
        }
        if(bin[0]=='1')
            dec-=pow2;
        return dec;
    }
    string DecToBin(long dec)
    {
        string bin="";
        long n=abs(dec);
        while(n>0)
        {
            bin+=('0'+(n%2));
            n/=2;
        }
        reverse(bin.begin(),bin.end());
        bin="0"+bin;
        if(dec<0)   //for 2s complement
        {
            n=bin.length();
            bool flag1=false;
            for(int i=n-1;i>=0;i--)
            {
                if(flag1)
                    bin[i]='0'+'1'-bin[i];
                else if(bin[i]=='1')
                    flag1=true;
            }
        }
        bin=SignedExtend32(bin);
        return bin;
    }
    FirstStage PC;
    SecondStage IFID;
    ThirdStage IDEX;
    FourthStage EXMO;
    FifthStage MOWB;
    vector<string> InstMem;
    vector<string> GPR;
    vector<string> DataMem;
    PipeLineOperandForwarding()
    {
        InstMem=vector<string> (MemSize,"");
        DataMem=vector<string> (MemSize,"");
        GPR=vector<string> (32,SignedExtend32("0"));
    }
    ControlWord ControlUnit(string opcode)
    {
        ControlWord CW;
        if(opcode=="0110011")   //R Type
        {
            CW.RegWrite=true;
            CW.AluOp='R';
        }
        else if(opcode=="0010011")   //I Type
        {
            CW.AluSrc=true;
            CW.RegWrite=true;
            CW.AluOp='I';
        }
        else if(opcode=="0000011")   //L Type
        {
            CW.AluSrc=true;
            CW.Mem2Reg=true;
            CW.RegWrite=true;
            CW.MemRead=true;
            CW.AluOp='L';
        }
        else if(opcode=="0100011")   //S Type
        {
            CW.MemWrite=true;
            CW.AluOp='S';
        }
        else if(opcode=="1100011")  //B Type
        {
            CW.Branch=true;
            CW.AluOp='B';
        }
        else if(opcode=="1101111")   //JAL
        {
            CW.RegWrite=true;
            CW.Jump=true;
            CW.AluOp='J';
        }
        else if(opcode=="1100111")  //JALR
        {
            CW.AluSrc=true;
            CW.RegWrite=true;
            CW.Jump=true;
            CW.AluOp='I';
        }
        else if(opcode=="0110111")   //LUI
        {
            CW.AluSrc=true;
            CW.RegWrite=true;
            CW.AluOp='U';
        }
        else if(opcode=="0010111")  //AUIPC
        {
            CW.AluSrc=true;
            CW.RegWrite=true;
            CW.AluOp='A';
        }
        return CW;
    }
    char ALUControlUnit(string func3,char func7)
    {
        char AluOp=IDEX.CW.AluOp;
        char operation=';';
        if(AluOp=='R')
        {
            if(func7=='1')
            {
                if(func3=="000")
                    operation='-';
                else if(func3=="001")
                    operation='*';
                else if(func3=="010")
                    operation='/';
                else if(func3=="011")
                    operation='%';
                else if(func3=="101")
                    operation='r';
            }
            else
            {
                if(func3=="000")
                    operation='+';
                else if(func3=="001")
                    operation='<';
                else if(func3=="010")
                    operation='(';
                else if(func3=="011")
                    operation='[';
                else if(func3=="100")
                    operation='^';
                else if(func3=="101")
                    operation='>';
                else if(func3=="110")
                    operation='|';
                else if(func3=="111")
                    operation='&';
            }
        }
        else if(AluOp=='I')
        {
            if(func3=="000")
                operation='+';
            else if(func3=="010")
                operation='(';
            else if(func3=="011")
                operation='[';
            else if(func3=="100")
                operation='^';
            else if(func3=="110")
                operation='|';
            else if(func3=="111")
                operation='&';
            else if(func3=="001")
                if(func7=='0')
                    operation='<';
            else if(func3=="101")
                if(func7=='0')
                    operation='>';
                else
                    operation='r';
        }
        else if(AluOp=='L')
        {
            operation='+';
        }
        else if(AluOp=='S')
        {
            operation='#';
        }
        else if(AluOp=='B')
        {
            if(func3=="000")
                operation='=';
            else if(func3=="001")
                operation='!';
            else if(func3=="100")
                operation='<';
            else if(func3=="101")
                operation='>';
            else if(func3=="110")
                operation='(';
            else if(func3=="111")
                operation=')';
        }
        else if(AluOp=='U'||AluOp=='A')
        {
            operation='+';
        }
        return operation;
    }
    pair<bool,string> ALU(bool AluSrc,char AluOperation,bool isBranch)
    {
        long input1,input1U;
        long input2,input2U;
        input1=BinToDec(IDEX.RS1);
        input1U=BinToDec("0"+IDEX.RS1);
        if(AluSrc)
        {
            string Imm=IDEX.IMMalu;
            if(AluOperation=='<'||AluOperation=='>'||AluOperation=='r')
                Imm=Imm.substr(12-5,5);
            input2=BinToDec(Imm);
            input2U=BinToDec("0"+Imm);
        }
        else
        {
            string RS2=IDEX.RS2;
            if(AluOperation=='#')
                RS2=IDEX.IMMbr;
            input2=BinToDec(RS2);
            input2U=BinToDec("0"+RS2);
        }
        long output=0;
        bool branchFlag=false;
        long msb=input1-(input1%(1<<31));
        if(!isBranch)
        {
            switch(AluOperation)
            {
                case '+':
                    output=input1+input2;
                    break;
                case '#':
                    output=input1+input2;
                    break;
                case '-':
                    output=input1-input2;
                    break;
                case '*':
                    output=input1*input2;
                    break;
                case '/':
                    output=input1/input2;
                    break;
                case '%':
                    output=input1%input2;
                    break;
                case '&':
                    output=input1&input2;
                    break;
                case '|':
                    output=input1|input2;
                    break;
                case '^':
                    output=input1^input2;
                    break;
                case '<':
                    output=input1<<input2;
                    break;
                case '>':
                    output=input1>>input2;
                    break;
                case 'r':
                    output=input1;
                    while(input2)
                    {
                        input2--;
                        input1=msb+input1<<1;
                    }
                    break;
                case '(':
                    output=input1<input2;
                    break;
                case '[':
                    output=input1U<input2U;
                    break;
            }
        }
        else
        {
            switch(AluOperation)
            {
                case '=':
                    branchFlag=(input1==input2);
                    break;
                case '!':
                    branchFlag=(input1!=input2);
                    break;
                case '<':
                    branchFlag=(input1<input2);
                    break;
                case '>':
                    branchFlag=(input1>=input2);
                    break;
                case '(':
                    branchFlag=(input1U<input2U);
                    break;
                case ')':
                    branchFlag=(input1U>=input2U);
                    break;
            }
        }
        string Output=DecToBin(output);
        return {branchFlag,Output};
    }
    char DataSizeControlUnit(string func3)
    {
        char size='\0';
        if(func3=="000")
            size='B';
        else if(func3=="001")
            size='H';
        else if(func3=="010")
            size='W';
        else if(func3=="100")
            size='b';
        else if(func3=="101")
            size='h';
        return size;
    }
    string DataSize(string data,char size)
    {
        string ans=data;
        switch(size)
        {
            case 'B':
                ans=ans.substr(0,8);
                break;
            case 'b':
                ans="0"+ans.substr(0,8);
                break;
            case 'H':
                ans=ans.substr(0,16);
                break;
            case 'h':
                ans="0"+ans.substr(0,16);
                break;
            case 'W':
                ans=ans.substr(0,32);
                break;
        }
        ans=ZeroExtend32(ans);
        return ans;
    }
    void InstructionFetch()
    {
        if(IFID.stall)
            return;
        if(PC.valid)
        {
            if(PC.pc<0||PC.pc>=MemSize)
            {
                IFID.valid=false;
                PC.valid=false;
                return;
            }
            IFID.IR=InstMem[PC.pc];
            IFID.DPC=PC.pc;
            if(InstMem[PC.pc]=="")
            {
                IFID.valid=false;
                PC.valid=false;
            }
            else
                IFID.valid=true;
            PC.pc=PC.pc+1;
        }
    }
    void InstructionDecode()
    {
        GPR[0]=SignedExtend32("0"); //Hardwiring R0 to 0
        if(IDEX.stall)
        {
            IFID.stall=true;
            return;
        }
        else
        {
            IFID.stall=false;
        }
        if(IFID.valid)
        {
            string imm20=(IFID.IR).substr(31-31,20);
            string func3=(IFID.IR).substr(31-14,3);
            string Rdl=(IFID.IR).substr(31-11,5);
            string controlword=(IFID.IR).substr(31-6,7);
            string Rs1=(IFID.IR).substr(31-19,5);
            string Rs2=(IFID.IR).substr(31-24,5);
            IDEX.Rs1=Rs1;
            IDEX.Rs2=Rs2;
            IDEX.Rdl=Rdl;
            IDEX.DPC=IFID.DPC;
            IDEX.JPC=IFID.DPC+BinToDec(imm20);
            IDEX.Func=func3;
            IDEX.CW=ControlUnit(controlword);
            int rs1=BinToDec("0"+Rs1);
            int rs2=BinToDec("0"+Rs2);
            IDEX.RS1=GPR[rs1];
            IDEX.RS2=GPR[rs2];
            IDEX.IMMalu=(IFID.IR).substr(31-31,12);
            int rdl=BinToDec("0"+Rdl);
            if(IDEX.CW.AluOp=='U'||IDEX.CW.AluOp=='A')
                rs1=0;
            if(IDEX.CW.AluOp=='U')
            {
                IDEX.IMMalu=ZeroExtend32(imm20);
                IDEX.RS1=DecToBin(0);
                IDEX.Rs1="";
            }
            else if(IDEX.CW.AluOp=='A')
            {
                IDEX.IMMalu=ZeroExtend32(imm20);
                IDEX.RS1=DecToBin(IFID.DPC);
                IDEX.Rs1="";
            }
            IDEX.IMMbr=(IFID.IR).substr(31-31,7)+(IFID.IR).substr(31-11,5);
            IDEX.valid=true;
        }
        else
        {
            IDEX.valid=false;
        }
    }
    void InstructionExecute()
    {
        if(EXMO.stall)
        {
            IDEX.stall=true;
            EXMO.valid=false;
            return;
        }
        else
        {
            IDEX.stall=false;
        }
        if(IDEX.valid&&!EXMO.stall)
        {
            char func30=IDEX.IMMalu[1];
            char AluSelect=ALUControlUnit(IDEX.Func,func30);
            pair <bool,string> receivefromAlu=ALU(IDEX.CW.AluSrc,AluSelect,IDEX.CW.Branch);
            string AluOut=receivefromAlu.second;
            bool branchFlag=receivefromAlu.first;
            EXMO.CW=IDEX.CW;
            EXMO.ALUOUT=AluOut;
            EXMO.RS2=IDEX.RS2;
            EXMO.Rdl=IDEX.Rdl;
            EXMO.DataControl=DataSizeControlUnit(IDEX.Func);
            EXMO.valid=true;
            int BPC=IDEX.DPC+BinToDec(IDEX.IMMbr);
            if(IDEX.CW.Branch&&branchFlag)
            {
                IFID.valid=false;
                EXMO.valid=false;
                PC.pc=BPC;
                PC.valid=true;
            }
            if(IDEX.CW.Jump)
            {
                IFID.valid=false;
                if(IDEX.CW.AluOp=='I')
                {
                    IDEX.JPC=BinToDec(AluOut);
                }
                EXMO.ALUOUT=DecToBin(IDEX.DPC+1);
                EXMO.valid=true;    //to write NPC in given register
                PC.pc=IDEX.JPC;
                PC.valid=true;
            }
        }
        else
        {
            EXMO.valid=false;
        }
    }
    void MemoryOperation()
    {
        if(EXMO.valid)
        {
            if(EXMO.CW.MemRead||EXMO.CW.MemWrite)
            {
                int effectiveAddress=BinToDec(EXMO.ALUOUT);
                if(effectiveAddress<0||effectiveAddress>=MemSize)
                {
                    cout<<"Invalid Address for Data Memory"<<endl;
                    return;
                }
                if(EXMO.CW.MemWrite)
                {
                    DataMem[effectiveAddress]=DataSize(EXMO.RS2,EXMO.DataControl);
                }
                if(EXMO.CW.MemRead)
                {
                    MOWB.LDOUT=DataSize(DataMem[effectiveAddress],EXMO.DataControl);
                }
            }
            MOWB.CW=EXMO.CW;
            MOWB.ALUOUT=EXMO.ALUOUT;
            MOWB.Rdl=EXMO.Rdl;
            if(EXMO.CW.RegWrite)
            {
                if(EXMO.CW.Mem2Reg)
                    EXMO.stall=true;
                if(EXMO.Rdl==IDEX.Rs1)
                    IDEX.RS1=EXMO.ALUOUT;
                if(!IDEX.CW.AluSrc&&(EXMO.Rdl==IDEX.Rs2))
                    IDEX.RS2=EXMO.ALUOUT;
                MOWB.valid=true;
            }
            else
            {
                MOWB.valid=false;
            }
        }
        else
        {
            MOWB.valid=false;
        }
    }
    void WriteBack()
    {
        if(MOWB.valid)
        {
            string writeData;
            if(MOWB.CW.Mem2Reg)
            {
                writeData=MOWB.LDOUT;
            }
            else
            {
                writeData=MOWB.ALUOUT;
            }
            if(MOWB.CW.RegWrite)
            {
                int rdl=BinToDec("0"+MOWB.Rdl);
                GPR[rdl]=writeData;
                if(MOWB.Rdl==IDEX.Rs1)
                    IDEX.RS1=writeData;
                if(!IDEX.CW.AluSrc&&(MOWB.Rdl==IDEX.Rs2))
                    IDEX.RS2=writeData;
            }
            EXMO.stall=false;
        }
    }
    void PrintMem()
    {
        int data=0;
        cout<<"General Purpose Registers:"<<endl;
        for(int i=0;i<32;i++)
        {
            data=BinToDec(GPR[i]);
            if(data!=0||i==0)
                cout<<"R"<<i<<": "<<data<<endl;
        }
        cout<<"\nData Memory:"<<endl;
        for(int i=0;i<MemSize;i++)
        {
            data=BinToDec(DataMem[i]);
            if(data!=0)
                cout<<"DM"<<i<<": "<<data<<endl;
        }
        cout<<endl;
    }
    void main(vector<string> &BinarySegmented)
    {
        cout<<underline<<"Instruction Memory:"<<closeunderline<<endl;
        for(int i=0;i<MemSize;i++)
        {
            InstMem[i]=BinarySegmented[i];
            if(InstMem[i]!="")
                cout<<i<<". "<<InstMem[i]<<endl;
        }
        cout<<endl<<underline<<"Simulating CPU:"<<closeunderline<<endl;
        bool validity=PC.valid=true;
        int cycle=1;
        while(validity)
        {
            sleep(1);
            cout<<underline<<"Cycle "<<cycle++<<":"<<closeunderline<<endl;
            WriteBack();
            MemoryOperation();
            InstructionExecute();
            InstructionDecode();
            InstructionFetch();
            PrintMem();
            validity=PC.valid||IFID.valid||IDEX.valid||EXMO.valid||MOWB.valid;
        }
    }
};
void Convert1toMany(string &bin,vector<string> &BinSeg)
{
    int l=bin.length();
    int IC=0;
    string instruction="";
    for(int i=0;i<l&&IC<MemSize;i++)
    {
        if(bin[i]=='\n')
        {
            if(instruction!="")
            {
                BinSeg[IC]=instruction;
                IC+=1;
            }
            instruction="";
        }
        else
        {
            instruction+=bin[i];
        }
    }
}
int main()
{
    Assembler Phase1;
    string bin=Phase1.main();
    int choice;
    cout<<"Enter Choice of CPU:"<<endl;
    cout<<"1. Stall\t2. Operand Forwarding"<<endl;
    cin>>choice;
    vector<string> BinarySegmented(MemSize);
    Convert1toMany(bin,BinarySegmented);
    if(choice<2)
    {
        PipeLineStall Phase2;
        Phase2.main(BinarySegmented);
    }
    else
    {
        PipeLineOperandForwarding Phase2;
        Phase2.main(BinarySegmented);
    }
    return 0;
}