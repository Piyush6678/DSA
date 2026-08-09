#include<iostream>
#include<stack>
#include<vector>
#include<algorithm>
using namespace std;
bool balancedBrackets(string s){
    if(s.length() %2!=0) return false;
    stack<char> stk;
    for (int i =0;i<s.length();i++){
if(s[i]=='('){
    stk.push('(');
}
else{
    if(stk.size()==0)return false;
    else stk.pop();
}

    }
    if(stk.size()==0)return true;
    return false;

}
//Remove consectutive duplicates in a string
string removeConsecutiveDuplicate(string s ){
    stack<char> stk;
    for(int i=0;i<s.length();i++ ){
        if(stk.size()==0 || stk.top()!=s[i] )stk.push(s[i]);
      
    }
    s="";
    while(stk.size()){
s+=(stk.top());
stk.pop();
    }
 reverse(s.begin(),s.end());
     return s;
}
//next greater element
vector<int>  nxtgrtest(int arr[],int n){
vector <int> ans(n);
stack<int> help;
ans[n-1]=-1;
help.push(arr[n-1]);
for (int i =n-2;i>=0;i--){
while( help.size() &&  help.top()<arr[i]   )help.pop();
if(help.size()==0)ans[i]=-1;
else ans[i]=help.top();
help.push(arr[i]);
}return ans;
}
//prev greater element
vector<int>  prevgrtest(int arr[],int n){
vector <int> ans(n);
stack<int> help;
ans[0]=-1;
help.push(arr[0]);
for (int i =1;i<n;i++){
while( help.size() &&  help.top()<arr[i]   )help.pop();
if(help.size()==0)ans[i]=-1;
else ans[i]=help.top();
help.push(arr[i]);
}return ans;
}
// design a stack get Minimum( shoulds be O(1)
void pushHelper(stack<int>&o,stack<int>&m){
  int n ;
    for(int i=1;i<=5;i++){
        cin>>n;
        o.push(n);
        //O(n) space complexity
        if(m.empty() || m.top()>n){
            m.push(n);
        }else m.push(m.top());
        //O(1) space complexity
        if(m.empty() || m.top()>=n){
            m.push(n);
        };
    }
}


int get_min(){
// pushing iin stack 
stack<int>org;
stack<int>min;
pushHelper(org,min);
return min.top();


}
//check if a list/string is plindrome or not means no moving backward a aspecial character X is marked as the middle of the string
bool isPalindrome(string s){
    stack<char>st;
int i =0;
int a =0;// checks if X is encountered or not
    while(s[i]){
if(s[i]=='X')a=1;
else if(a=0) st.push(s[i]);
else{
    if(s[i]==st.top())st.pop();
    else return false;
}
}return true;

}


// m stack using an array 
// divide arrau in m parts each can store n/m elements if array is of n size
// finding spans
vector<int> calculateSpan(int A[], int n) {
    vector<int> spans(n);

    // Iterate over every element in the array
    for (int i = 0; i < n; i++) {
        int currentSpan = 1; // The span always includes the current day itself

        // Traverse backwards from the current element
        for (int j = i - 1; j >= 0; j--) {
            // If the previous element is smaller or equal, extend the span
            if (A[j] <= A[i]) {
                currentSpan++;
            } 
            // If we find a greater element, stop counting
            else {
                break;
            }
        }
        
        spans[i] = currentSpan;
    }

    return spans;
}
vector<int> calculateSpan_using_stack(int A[], int n) {
    vector<int> spans(n);
    stack<int> s; // Stores INDICES, not values

    for (int i = 0; i < n; i++) {
        // Step 1: Pop elements from stack while stack top is smaller than current A[i]
        // We use a WHILE loop because we might need to pop multiple small elements
        while (!s.empty() && A[s.top()] <= A[i]) {
            s.pop();
        }

        // Step 2: Calculate Span
        if (s.empty()) {
            // If stack is empty, A[i] is the largest so far
            spans[i] = i + 1;
        } else {
            // Distance between current index and the index of the nearest greater element
            spans[i] = i - s.top();
        }

        // Step 3: Push current index to stack
        s.push(i);
    }

    return spans;
}



int main(){
    string s="()(())()((())))";
    string s2="adsfaasafewffgggsae";
    cout<<balancedBrackets(s);
    return 0;
}