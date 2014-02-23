#include <iostream>




using namespace std;


float cofactor(vetor<float >& v){
	return (v[4]*v[8]-v[5]*v[7])*v[0]+(v[5]*v[6]-v[3]*v[8])*v[1]+(v[3]*v[7]-v[4]*v[6])*v[2];



}
vector<float> mat(vector<float> & v2){
	vector<float> vReturn(16,0);
	for(int i=0;i<16;i++)
		for(int j=0;j<16;j++){
			for(int l=0;l<16;l++)
				for(int k=0;k<16;k++){
					vector<float> v;
					if(k!=i && l!=j)
						v.push_back(v2[k][l]);
					vReturn[j][i]=cofactor(v);
				}
		}

}


int main(){

	vector<float> vReturn;
		for(int i=0;i<16;i++)
			for(int j=0;j<16;j++){
		}

	
}