#include <iostream>
#include <fstream>
#include <bitset>
#include <random>
#include <map>
#include <chrono>
#include <thread>
#include <string>

using namespace std;



namespace bit_math{
    bitset<2048> Addition(bitset<2048> a, bitset<2048> b){
        bitset<2048> carry = a & b;
        bitset<2048> result = a ^ b;
        while(carry != 0){
            bitset<2048> temp = carry << 1;
            carry = result & temp;
            result = result ^ temp;
        }
        return result;
    }

    bitset<2048> Subtract(bitset<2048> a, bitset<2048> b){
        // if (isLess(a, b))return 0;
        // cout << "a:   " << a << "       b:      " << b << endl;
        while(b.none() != 1){
            bitset<2048> borrow = (~a) & b;
            a = a ^ b;
            b = borrow << 1;
        }
        return a;
    }

    bitset<2048> Multiple(bitset<2048> a, bitset<2048> b){
        bitset<2048> result("0");
        bitset<1> mark_a(0), mark_b(0);
        //cout << a << endl << b << endl;
        if (a.test(1023) & 1 == 1){
            mark_a.set(0);
            a = a.flip();
            a = Addition(a, 1);
        }
        if (b.test(1023) & 1 == 1){
            mark_b.set(0);
            b = b.flip();
            b = Addition(b, 1);
        }
        //cout << mark_a << mark_b << endl;
        while(b.none() != 1){
            //cout << "CCCCCCCCCCCC" << result << endl;
            
            if (b.test(0) & 1){
                //cout << "TTTT" << result << endl << a << endl;
                result = bit_math::Addition(result, a);
            }
            a = a << 1;
            b = b >> 1;
        }
        if (mark_a.test(0) ^ mark_b.test(0)){
            result = Subtract(result, 1);
            result = result.flip();
        }
        return result;
    }

    int Find_MSB(bitset<2048> a){
        if(a.none() == 1){
            return 0;
        }
        int i = a.size() - 1;
        while(i != 0){
            if(a.test(i) == 1){
                return i;
            }
            i--;
        }
        return 0;
    }

    bool isLessEqual(bitset<2048> a, bitset<2048> b){
        bitset<2048> temp = a ^ b;
        int MSB = bit_math::Find_MSB(temp);
        if ((b.test(MSB)) == 1 || (MSB == 0 && a.test(MSB) == b.test(MSB))){
            return true;
        }
        return false;
    }

    bool isLess(bitset<2048> a, bitset<2048> b){
        bitset<2048> temp = a ^ b;
        int MSB = bit_math::Find_MSB(temp);
        if (temp.test(MSB) & b.test(MSB) == 1){
            return true;
        }
        return false;
    }

    bitset<2048> Square_root(bitset<2048> a){
        int msb = bit_math::Find_MSB(a);
        bitset<2048> temp = 1 << msb;
        bitset<2048> result("0");
        while(temp.none() != 1){
            bitset<2048> sum = bit_math::Addition(result, temp);
            if (isLessEqual(bit_math::Multiple(sum, sum), a)){
                result = bit_math::Addition(result, temp);
            }
            temp = temp >> 1;
        }
        return result;
    }

    bitset<2048> Division(bitset<2048> a, bitset<2048> b){
        bitset<2048> Ans("0");
        int size_a = Find_MSB(a);
        int size_b = Find_MSB(b) + 1;
        bitset<2048> temp;
        for (int i = size_a; i >= 0; i--){
            
            temp[0] = a[i];
            if (isLess(temp, b)){
                temp = temp << 1;
                Ans = Ans << 1;
            }
            else{
                temp = Subtract(temp, b);
                temp = temp << 1;
                Ans[0] = 1;
                Ans = Ans << 1;
            }
        }
        Ans = Ans >> 1;
        return Ans;
    }

    bitset<2048> Modulo(bitset<2048> a, bitset<2048> b){
        // cout << Division(a, b) << "LOLOL" << endl;
        return Subtract(a, Multiple(b, Division(a, b)));
    }

    bitset<2048> Power(bitset<2048> a, bitset<2048> b){
        bitset<2048> result(1);
        while (b.none() != 1) {
 
        bool last_bit = (b.test(0) & 1);
        if (last_bit) {
            result = Multiple(result, a);
        }
 
        a = Multiple(a, a);
        b = b >> 1;
        }
        return result;
    }
}

void reverStr(string& str){
    int n = str.length();
    for (int i = 0; i < n / 2; i++){
        swap(str[i], str[n - i - 1]);
    }
}

void Read_File(string Text[], const char* File){
    fstream MyFile;
    string line;
    int count = 0;
    MyFile.open(File);
    while(getline(MyFile, line)){
        Text[count] = line;
        count ++;
    }
    MyFile.close();
    return;
}

void Write_File(string Text, const char* File){
    ofstream MyFile;
    MyFile.open(File);
    MyFile << Text;
    MyFile.close();
    return;
}

string Hex_to_Bytes(char c){
    switch(toupper(c))
    {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': return "1010";
        case 'B': return "1011";
        case 'C': return "1100";
        case 'D': return "1101";
        case 'E': return "1110";
        case 'F': return "1111";
    }
    return NULL;
}

string Bytes_to_Hex(string str){
    map<std::string, string> map;
    map["0000"] = '0';
    map["0001"] = '1';
    map["0010"] = '2';
    map["0011"] = '3';
    map["0100"] = '4';
    map["0101"] = '5';
    map["0110"] = '6';
    map["0111"] = '7';
    map["1000"] = '8';
    map["1001"] = '9';
    map["1010"] = 'A';
    map["1011"] = 'B';
    map["1100"] = 'C';
    map["1101"] = 'D';
    map["1110"] = 'E';
    map["1111"] = 'F';
    return map[str];
}

void Num_to_Bit(string& Num){
    string newNum;
    int n = Num.length();
    for (int i = 0; i < n; i++){
        string temp = Hex_to_Bytes(Num[i]);
        newNum.append(temp);
    }
    Num = newNum;
}

void Bit_to_Num(string& Bit){
    string result;
    bool flag = true;
    for(int i = 0; i < Bit.length(); i+=4){
        string Hex = Bytes_to_Hex(Bit.substr(i, 4));
        if (flag){
            if (Hex == "0"){
                continue;
            }
            else flag = false;
        }
        result.append(Hex);
    }
    Bit = result;
}

bitset<2048> random_bitset(int MSB, double p = 0.5) {
    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution d(0.5);
    bitset<2048> t;
    for (int i = 0; i < MSB; i++){
        t[i] = d(gen);
    }
    return t;
} 

bitset<2048> phi_Euler(bitset<2048> p, bitset<2048> q){
    p = bit_math::Subtract(p, 1);
    q = bit_math::Subtract(q, 1);
    return bit_math::Multiple(p, q);
}

bitset<2048> Exten_GCD(bitset<2048> a, bitset<2048> b, bitset<2048> old_s, bitset<2048> s, bitset<2048>& temp){
    while(b.none() != 1){
        bitset<2048> remind_1 = old_s;
        old_s = s;
        s = bit_math::Subtract(remind_1, bit_math::Multiple(bit_math::Division(a, b), old_s));
        bitset<2048> remind_2 = a;
        a = b;
        b = bit_math::Modulo(remind_2, b);
    }
    temp = old_s;
    return a;
}

string Encode_Bit(bitset<2048> N, bitset<2048> k, bitset<2048> x){
    bitset<2048> result(1);
    bitset<2048> arr[2048];
    arr[0] = bit_math::Modulo(x, N); 
    result = bit_math::Multiple(result, arr[0]);
    for (int i = 1; i <= bit_math::Find_MSB(k); i++){
        arr[i] = bit_math::Modulo(bit_math::Multiple(arr[i - 1], arr[i - 1]), N);
        if (k.test(i) & 1 == 1){
            result = bit_math::Multiple(result, arr[i]);
        }
        result = bit_math::Modulo(result, N);
    }
    result = bit_math::Modulo(result, N);
    string res = result.to_string();
    Bit_to_Num(res);
    reverStr(res);
    return res;
}


void Task(char** argv){
    string myText[3];
    Read_File(myText, argv[1]);
    for (int i = 0; i < 3; i++){
        reverStr(myText[i]);
        Num_to_Bit(myText[i]);
    }
    bitset<2048> N(myText[0]), k(myText[1]), x(myText[2]);
    string result =  Encode_Bit(N, k, x);
    Write_File(result, argv[2]);
    exit(0);
}

int main(int argc, char **argv){
    const int time = 60;
    thread n_thresh(Task, argv);
    this_thread::sleep_for(chrono::seconds(time));
    return 0;
}
