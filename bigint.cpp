#include <iostream>
#include <string>
#include <cmath>
#include "bigint.h"

using namespace std;

// Write your implementation below
BigInt::BigInt(string s, int base){
    int i=0;
    while(s[0] == '0' && s.length()>1){
        s.erase(0,1);
        i++;
    }
    for(int i=s.length()-1; i>=0; i--){
        if(int(s[i])>=65){
            // its a letter
            numinlist.push_back(10+static_cast<int>(s[i]-'A'));
        } else {
            // its number
            numinlist.push_back(static_cast<int>(s[i]-'0'));
        }
    }
    baseinfo = base;
} // convert string to BigInt
string BigInt::to_string() const{
    string ans = "";
    int x = numinlist.size();
    for(int i=x-1; i>=0; i--){
        if(numinlist.at(i)>10){
            // over 10 num to alphabet
            ans += (char)(numinlist[i]-10+'A');
        } else{
            // less than 1
            ans += (char)(numinlist[i]+'0');
        }
    } 
    return ans;
} // get string representation
void BigInt::add(BigInt b){
    vector<int> carry;
    vector<int> num2; 
    numinlist.push_back(0);
    string temp = b.to_string();
    for(int i = temp.length()-1; i>=0; i--){
        if(int(temp[i])>=65){
            // its a letter
            num2.push_back(10+static_cast<int>(temp[i]-'A'));
        } else {
            // its a number
            num2.push_back(static_cast<int>(temp[i]-'0'));
        }
    }
    num2.push_back(0);
    carry.assign(max(numinlist.size(), num2.size()),0);
    int n1size = numinlist.size();
    int n2size = num2.size();
    if(n1size < n2size){
        for(int i=n1size; i<n2size; i++){
            numinlist.push_back(0);
        }
    } else if(n1size > n2size){
        for(int i=n2size; i<n1size; i++){
            num2.push_back(0);
        }
    }


    int vecsize = numinlist.size();
    for(int i=0; i<vecsize; i++){
        if(carry.at(i) + numinlist.at(i) + num2.at(i) >= baseinfo){
            numinlist.at(i) = ((carry.at(i) + numinlist.at(i) + num2.at(i)) - baseinfo);
            carry.at(i+1) = 1;
        } else{
            numinlist.at(i) = ((carry.at(i) + numinlist.at(i) + num2.at(i)));
        }
    }

    if(numinlist.at(numinlist.size()-1) == 0){
        int i = numinlist.size()-1;
        numinlist.erase(numinlist.begin()+i);
    }
} // add another BigInt to this one


// returns the sum of this BigInt and rhs
BigInt BigInt::operator+(const BigInt& rhs) const{
    BigInt temp = *this;
    temp.add(rhs);
    return temp;
}
// returns the difference of this BigInt minus rhs
BigInt BigInt::operator-(const BigInt& rhs) const{
    BigInt temp = *this;
    vector<int> ans;
    vector<int> borrow;
    vector<int> tempcopy = temp.numinlist;
    vector<int> rhscopy = rhs.numinlist;
    //temp.numinlist
    borrow.assign(max(numinlist.size(), rhs.numinlist.size()),0);
    int n1size = tempcopy.size();
    int n2size = rhscopy.size();
    if(n1size < n2size){
        for(int i=n1size; i<n2size; i++){
            tempcopy.push_back(0);
        }
    } else if(n1size > n2size){
        for(int i=n2size; i<n1size; i++){
            rhscopy.push_back(0);
        }
    }

    int vecsize = tempcopy.size();
    //BigInt b1("103"), b2("98");
    //301
    //89
    for(int i=0; i<vecsize; i++){
        if(tempcopy.at(i) - rhscopy.at(i) - borrow.at(i)>=0){
            if ((i != vecsize-1) || (i == vecsize-1 && ans.size() == 0) || (i == vecsize-1 && tempcopy.at(i) - rhscopy.at(i)-borrow.at(i) != 0)){
                //cout << "adding pattern 1 - " << tempcopy.at(i) - rhscopy.at(i)-borrow.at(i) << endl;
                ans.push_back(tempcopy.at(i) - rhscopy.at(i)-borrow.at(i));
            }
        } else if(tempcopy.at(i) - rhscopy.at(i) - borrow.at(i)<0){
            borrow.at(i+1) = 1;
            if((i != vecsize-1) || (i == vecsize-1 && ans.size() == 0) || (i == vecsize-1 && 10+tempcopy.at(i) - rhscopy.at(i)-borrow.at(i) != 0)){
                //cout << "adding pattern 2 - " << 10+tempcopy.at(i) - rhscopy.at(i)-borrow.at(i) << endl;
                ans.push_back(10+tempcopy.at(i) - rhscopy.at(i)-borrow.at(i));
            }
        }
    }
    temp.numinlist = ans;
    return temp;
}
// returns the true if this BigInt is less than rhs
bool BigInt::operator<(const BigInt& rhs) const{
    BigInt temp = *this;
    int size1, size2;
    size1 = temp.numinlist.size();
    size2 = rhs.numinlist.size();
    if(size1>size2){
        return false;
    } else if(size1<size2){
        return true;
    } else if(size1 == size2){
        int count=0;
        while(count != size1){
            if(temp.numinlist.at(size1-1-count) != rhs.numinlist.at(size2-1-count)){
                return (temp.numinlist.at(size1-1-count) < rhs.numinlist.at(size2-1-count));
            }
            count++;
        }
    }
    return false;
}

    /* int main(){
        BigInt b1("98"), b2("98");
        cout << boolalpha;
        cout << (b1 < b2) << endl; // should print true
        cout << (b2 < b1) << endl; // should print false
        return 0;
    }
 */
  