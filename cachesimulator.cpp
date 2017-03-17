/*
Cache Simulator
Level one L1 and level two L2 cache parameters are read from file (block size, line per set and set per cache).
The 32 bit address is divided into tag bits (t), set index bits (s) and block offset bits (b)
s = log2(#sets)   b = log2(block size)  t=32-s-b
*/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <bitset>

using namespace std;
//access state:
#define NA 0 // no action
#define RH 1 // read hit
#define RM 2 // read miss
#define WH 3 // Write hit
#define WM 4 // write miss




struct config{
       int L1blocksize;
       int L1setsize;
       int L1size;
       int L2blocksize;
       int L2setsize;
       int L2size;
       };

class check_cache
{
public: int s,b,t;
    // cache_struct sa[];
    
    
    check_cache( int blocksize ,int setsize ,int size)
    {
        //cache_struct dm;
        //cache r;
        //int s,b,t;
        if (setsize == 0)
        {
            cout<<"full asso\n"<<endl;        // still have to implement
            b = log2(blocksize);
            cout<<b<<"  offset\n"<<endl;
            s = log2(size) + 10 - log2(blocksize) ;    // removed set size for further modification check if it is needed
            cout<<s<<" index \n"<<endl;
            t= 32-s-b;
            cout<<t<<" Tag\n"<<endl;
        }
        else if ( setsize == 1)
        {
            
            cout<<"Direct mapped\n"<<endl;
            b = log2(blocksize);
            cout<<b<<"  offset\n"<<endl;
            s = log2(size) + 10 - log2(blocksize) - log2(setsize); //remove set size
            cout<<s<<" index \n"<<endl;
            t= 32-s-b;
            cout<<t<<" Tag\n"<<endl;
            //cache_struct dm;
            //dm.cache_struct1(b, s);
            //sa.cache_struct1(b,s);
            //double data_array[(int)pow(2,s)][b];
            //cout<<(int)pow(2,s)<<"\n"<<endl;
            
        }
        else if (log2(setsize)==floor(log2(setsize)))    //check    log2(setsize)==floor(log2(setsize))    Or setsize%2==0
        {
            cout<<"Set Associativity\n"<<endl;
            b = log2(blocksize);
            cout<<b<<"  offset\n"<<endl;
            s = log2(size) + 10 - log2(blocksize) - log2(setsize);
            cout<<s<<" index \n"<<endl;
            t= 32-s-b;
            cout<<t<<" Tag\n"<<endl;
            //cache_struct sa[setsize];           //out of bounds?
            
            // int tag_array[(int)pow(2,s)];          //check the data type boolean ? / int ?
            // bool valid_array[(int)pow(2,s)];
            
            //error debugginh
        }
    }
};

int main(int argc, char* argv[]){


    
    config cacheconfig;
    ifstream cache_params;
    string dummyLine;
    cache_params.open(argv[1]);
    while(!cache_params.eof())  // read config file
    {
      cache_params>>dummyLine;
      cache_params>>cacheconfig.L1blocksize;
      cache_params>>cacheconfig.L1setsize;              
      cache_params>>cacheconfig.L1size;
      cache_params>>dummyLine;              
      cache_params>>cacheconfig.L2blocksize;           
      cache_params>>cacheconfig.L2setsize;        
      cache_params>>cacheconfig.L2size;
      }
    
  
  
   // Implement by you: 
   // initialize the hirearch cache system with those configs
   // probably you may define a Cache class for L1 and L2, or any data structure you like
    check_cache mycacheL1( cacheconfig.L1blocksize , cacheconfig.L1setsize , cacheconfig.L1size);
    check_cache mycacheL2( cacheconfig.L2blocksize , cacheconfig.L2setsize , cacheconfig.L2size);
    int pL1 = pow(2, mycacheL1.s);
    int qL2 = pow(2,mycacheL2.s);
    cout<< pL1 << "The index of L1 cache in tag\n";
    cout<< qL2 << "The index of L2 cache in tag\n";
    int tag_arrayL1[pL1 - 1][cacheconfig.L1setsize];
    int tag_arrayL2[qL2-1][cacheconfig.L2setsize];
    
    int valid_arrayL1[pL1 - 1][cacheconfig.L1setsize];
    int valid_arrayL2[qL2-1][cacheconfig.L2setsize];
    
    
   
   
   
  int L1AcceState =0; // L1 access state variable, can be one of NA, RH, RM, WH, WM;
  int L2AcceState =0; // L2 access state variable, can be one of NA, RH, RM, WH, WM;
   
   
    ifstream traces;
    ofstream tracesout;
    string outname;
    outname = string(argv[2]) + ".out";
    
    traces.open(argv[2]);
    tracesout.open(outname.c_str());
    
    string line;
    string accesstype;  // the Read/Write access type from the memory trace;
    string xaddr;       // the address from the memory trace store in hex;
    unsigned int addr;  // the address from the memory trace store in unsigned int;        
    bitset<32> accessaddr; // the address from the memory trace store in the bitset;
    
    if (traces.is_open()&&tracesout.is_open()){    
        while (getline (traces,line)){   // read mem access file and access Cache
            
            istringstream iss(line); 
            if (!(iss >> accesstype >> xaddr)) {break;}
            stringstream saddr(xaddr);
            saddr >> std::hex >> addr;
            accessaddr = bitset<32> (addr);
                                                // access the L1 and L2 Cache according to the trace;
                                                // cout<<accessaddr << "Address in binary\n";
                                                //cout<<xaddr << "Address in hex\n";
            int countL1 = 0 ;
            int countL2 = 0;
            int ta1 = mycacheL1.t;
            int ind1 = mycacheL1.s;
            int off1 = mycacheL1.b;  // new
            cout<<"this is the address in hex"<< xaddr <<"\n"<<endl;
            cout<<"this is the access address in hex"<< accessaddr <<"\n"<<endl;
            
            //bitset<32> tagl1 ((accessaddr.to_string().substr(0,ta1)));
            int tagL1 = (bitset<32>((accessaddr.to_string().substr(0,ta1))).to_ulong());
            cout<<tagL1<<"this is tag1 x in main for L1"<<endl;
            
            //bitset<32> indexl1 ((accessaddr.to_string().substr(ta1,ind1)));
            int indexL1 = (bitset<32>((accessaddr.to_string().substr(ta1,ind1))).to_ulong());
            cout<<indexL1<<"this is index1 x in main for L1"<<endl;
            
            //bitset<32> offl1 ((accessaddr.to_string().substr(ta1+ind1,off1)));
            int offL1 =(bitset<32>((accessaddr.to_string().substr(ta1+ind1,off1))).to_ulong());
            cout<<offL1<<"this is offset1 x in main for L1"<<endl;
            
            
            int ta2 = mycacheL2.t;
            int ind2 = mycacheL2.s;
            int off2=mycacheL2.b;  // new
            cout<<"this is the address in hex"<< xaddr <<"\n"<<endl;
            cout<<"this is the access address in hex"<< accessaddr <<"\n"<<endl;
            
            //bitset<32> tagl2 ((accessaddr.to_string().substr(0,ta2)));
            int tagL2 =  (bitset<32>((accessaddr.to_string().substr(0,ta2))).to_ulong());
            cout<<tagL2<<"this is tag2 x in main for L2"<<endl;
            
            //bitset<32> indexl2 ((accessaddr.to_string().substr(ta2,ind2)));
            int indexL2 (bitset<32>((accessaddr.to_string().substr(ta2,ind2))).to_ulong());
            cout<<indexL2<<"this is index2 x in main for L2"<<endl;
            
            //bitset<32> offl2(( accessaddr.to_string().substr(ta2+ind2,off2)));
             int offL2 (bitset<32>((accessaddr.to_string().substr(ta2+ind2,off2))).to_ulong());
            cout<<offL2<<"this is offset2 x in main for L2"<<endl;
            //
            if (accesstype.compare("R")==0)
            {
                 //Implement by you:
                 // read access to the L1 Cache, 
                 //  and then L2 (if required), 
                 //  update the L1 and L2 access state variable;
                 for(int jj = 0 ; jj<cacheconfig.L1setsize;jj++)
                 {
                     if((tag_arrayL1[indexL1][jj] == tagL1) && (valid_arrayL1[indexL1][jj] == 1))
                        {
                            L2AcceState =0;
                            L1AcceState =1;
                            cout<<" Read HIT NA \n";
                            break;
                        }
                    }
                  if(L1AcceState == 0)
                        {
                            L1AcceState = 2;
                            for(int jj = 0;jj<cacheconfig.L2setsize ; jj++)
                            {
                                if((tag_arrayL2[indexL2][jj]==tagL2) && (valid_arrayL2[indexL2][jj]==1))
                                   {
                                       L2AcceState = 1;
                                       for(int jj =0; jj < cacheconfig.L1setsize;jj++)
                                       {
                                           if(valid_arrayL1[indexL1][jj] == 0)
                                           {
                                               tag_arrayL1[indexL1][jj] = tagL1;
                                               valid_arrayL1[indexL1][jj] = 1;
                                               break;
                                           }
                                           else
                                           {
                                               jj = countL1;
                                               tag_arrayL1[indexL1][jj] = tagL1;
                                               countL1 =countL1 + 1;                //
                                               if(countL1 == cacheconfig.L1setsize)
                                               {
                                                   countL1=0;
                                               }
                                           }
                                       }
                                      break;
                                   }
                            }
                    if(L2AcceState==0)
                        {
                            L2AcceState = 2;
                            for(int jj = 0;jj < cacheconfig.L2setsize;jj++)
                            {
                            
                                if(valid_arrayL2[indexL2][jj]==0)
                                {
                                    tag_arrayL2[indexL2][jj] = tagL2;
                                    valid_arrayL2[indexL2][jj] = 1;
                                    //goto w;
                                    break;
                                }
                                else
                                {
                                    jj = countL2;
                                    tag_arrayL2[indexL2][jj] = tagL2;
                                    countL2 = countL2 + 1;
                                    if(countL2 == cacheconfig.L2setsize)
                                    {
                                        countL2 = 0;
                                    }
                                }
                            }
                            for(int jj = 0 ; jj < cacheconfig.L1setsize ; jj ++)
                            {
                                if(valid_arrayL1[indexL1][jj] == 0)              //tagarr
                                {
                                    tag_arrayL1[indexL1][jj] = tagL1;
                                    valid_arrayL1[indexL1][jj] =1;
                                    break;              //no break;
                                }
                                else
                                {
                                    jj = countL1;
                                    tag_arrayL1[indexL1][jj] = tagL1;
                                    countL1 += 1;
                                    if(countL1== cacheconfig.L1setsize)
                                    {
                                        countL1 = 0;
                                    }
                                }
                            }
                                
                        }
                                   
                    }
                }
                            
             else 
             {    
                   //Implement by you:
                  // write access to the L1 Cache, 
                  //and then L2 (if required), 
                  //update the L1 and L2 access state variable;
                 
                 for(int jj = 0 ; jj < cacheconfig.L1setsize ; jj ++)
                 {
                  
                     
                     if((tag_arrayL1[indexL1][jj] == tagL1) && (valid_arrayL1[indexL1][jj]==1))
                     {
                         L1AcceState = 3;
                         L2AcceState = 0;
                         cout<<"";
                         break;
                     }
                  }
                 //
                 if(L1AcceState == 0)
                 {
                 L1AcceState = 4;
                 for(int jj =0 ; jj < cacheconfig.L2setsize; jj++)
                 {
                     if((tag_arrayL2[indexL2][jj] == tagL2)&& (valid_arrayL2[indexL2][jj]==1))
                     {
                         L2AcceState = 3;
                         break;
                        // break;
                     }
                     
                 }
                   if(L2AcceState==0)
                     {
                       L2AcceState =4;
                        
                     }
                }
            
             }
            
              
              
            
        tracesout<< L1AcceState << " " << L2AcceState << endl;  // Output hit/miss results for L1 and L2 to the output file;
            L1AcceState = 0;
            L2AcceState = 0;
             
        }
        traces.close();
        tracesout.close(); 
    }
    else cout<< "Unable to open trace or traceout file ";


   
    
  

   
    return 0;
}
