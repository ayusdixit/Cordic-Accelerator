#include <iostream>
using namespace std; 
/////////////////////////////////////Lookup table 
#define atan0 45
#define atan1 	26.5623
#define atan21 14.0317  //since atan2 is keyword can't use it here 
#define atan3 7.1276
#define atan4 3.5753
#define atan5 1.7876
#define atan6 0.8938
#define atan7  0.4469
#define atan8  0.2235
#define atan9  0.1146
#define atan10 0.0573
#define atan11  0.0286
////////////////////////////////////////////
 void sanity_check(double  angle ,double  x_initial, double y_initial,double x_final,double y_final ,double z_initial,double  z_final , double sin_value ,double cos_value){
     cout<<"------------------------------------------------------------------" ;
   cout<<"angle is :"<<angle <<endl   ;
   cout<<"x_initial is :"<<x_initial <<endl  ;
     cout<<"y_initial is :"<<y_initial <<endl  ;
       cout<<"x_final is :"<<x_final <<endl  ;
         cout<<"y_final is :"<<y_final <<endl  ;
                cout<<"z_initial is :"<<z_initial <<endl  ;
         cout<<"z_final is :"<<z_final <<endl  ;
         cout<<"sin value is :"<<sin_value<<endl; 
         cout<<"cos value is :" <<cos_value<<endl; 
   }
int main()
{
  cout<<"cordic initialization";
  int n =13 ;   //no  of iteration 
  long double  x_initial  = 1 ;
  long double y_initial = 0 ; 
   long double x_final =0 ;
  long  double y_final  = 0; 
   long double z_initial = 0 ; 
   long double z_final  = 0 ; 
   bool sign_check = false;   // to check if it is positive and negative 
   double sin_value ; 
   double cos_value ; 
   
  long double overflow_no = 0.6072 ; 
   double angle ; 
   cout <<"enter the angle to be find "<<endl ;
   cin>>angle; 
    z_initial =angle  ;
       
   ////////////////////////////////////////////sanity check is here 
sanity_check(angle, x_initial, y_initial, x_final, y_final, z_initial, z_final,sin_value, cos_value) ; 
////////////////////////////////////////////
  float atan_table[] = {atan0, atan1, atan21, atan3, atan4, atan5, atan6, atan7, atan8, atan9, atan10, atan11};
    
      // CORDIC iterations
    for(int i = 0; i < n; i++) {
        // Replace bit shifts with division by power of 2
        float power_of_two = 1.0 / (1 << i);   
        
        if(z_initial >= 0) {
            x_final = x_initial - (y_initial * power_of_two);
            y_final = y_initial + (x_initial * power_of_two);
            z_final = z_initial - atan_table[i];
        } else {
            x_final = x_initial + (y_initial * power_of_two);
            y_final = y_initial - (x_initial * power_of_two);
            z_final = z_initial + atan_table[i];
        }
        
        // Print intermediate values for debugging
        cout << "\nIteration " << i << ":\n";
        cout << "x_final = " << x_final << endl;
        cout << "y_final = " << y_final << endl;
        cout << "z_final = " << z_final << endl;
        
        // Update values for next iteration
        x_initial = x_final;
        y_initial = y_final;
        z_initial = z_final;
    }
    if (n>=4)
    {
        sin_value = overflow_no*x_final ; 
        cos_value  =  overflow_no * y_final  ; 
    }
    sanity_check(angle, x_initial, y_initial, x_final, y_final, z_initial, z_final , sin_value , cos_value) ; 

    
    return 0;
    
}
