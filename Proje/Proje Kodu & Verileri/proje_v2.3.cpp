#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class dd{
	public:
	dd(double a=0 , double b=0): times(a), value(b){};
	
	void display(){
		cout<<times<<"			"<<value<<endl;
	};
	
	double times, value;
};

void write(vector<dd>& vec, fstream& file){
	
	for(size_t i=0 ; i<vec.size() ; i++)
		file<<vec[i].times<<"		"<<vec[i].value<<endl;
	
	file.close();
}

void BPMhesapla(vector<dd>& rt, string patient){
	
	double bpm;
	
	bpm = (60.0 / (rt[1].times - rt[0].times) );
	
	if (bpm < 60.0){
		fstream bradi(patient+"_Bradikardi.txt",ios::out);
		bradi<<"Time:		"<<"	Bradikardi Value:"<<endl;
		write(rt,bradi);
	}
	else if (bpm > 100.0){
		fstream tasi(patient+"_Taşikardi.txt",ios::out);
		tasi<<"Time:		"<<"	Taşikardi Value:"<<endl;

		write(rt,tasi);
	}
	else{
		fstream normal(patient+"_Normal.txt",ios::out);
		normal<<"Time:		"<<"	Normal Value:"<<endl;
		write(rt,normal);
	}

	
}

void rHesaplama(ifstream& file1,vector<dd>& rt){
	
	vector<dd> f1;
	double tim, val;
	
	ifstream timef("time.txt",ios::in);
	
	while(timef>>tim){
		file1>>val;
		dd nmb(tim, val);
		
		f1.push_back(nmb);
	}
	
	
	for (vector<dd>::iterator it1 = f1.begin() ; it1!=f1.end()-2 ; it1++) {
		
		while( (*it1).value < 0.04 && it1!=f1.end()-1)
			it1++;
		
		if(it1==f1.end())
			break;
		
		vector<dd>::iterator it2 = it1+1;
		
		double max = 0;
		while((*it2).value > 0.04 && it2!=f1.end()-1){
			if( (*it2).value > max){
				max = (*it2).value;
				it1 = it2;
			}
			it2++;
		}
		
		if(it2==f1.end())
			break;
		
		if((*it1).value > 0.05)
			rt.push_back( (*it1) );
		it1 = it2;

	}
	
	if (rt.empty()) {
		cout << "R bulunamadi";
		exit(1);
	}
	
	file1.close();
	timef.close();
}

void merge(string name){
	
	ifstream bradi(name+"_Bradikardi.txt");
	ifstream tasi(name+"_Taşikardi.txt");
	ifstream normal(name+"_Normal.txt");
	fstream merge(name+".txt",ios::out);
	
	if (!bradi || !tasi|| !normal || !merge) {
		cerr << "Error opening PATIENT file!" << endl;
		exit(3);
	}
	
	string t,v;
	
	merge<<"BRADİKARDİ\n----------\nTime:		Bradikardi Value:"<<endl;
	bradi>>t>>v>>v;
	while(bradi>>t>>v)
		merge<<t<<"			"<<v<<endl;
	merge<<"\n\n\n\n";
	
	merge<<"NORMAL\n----------\nTime:		Normal Value:"<<endl;
	normal>>t>>v>>v;
	while(normal>>t>>v)
		merge<<t<<"			"<<v<<endl;
	merge<<"\n\n\n\n";
	
	merge<<"TAŞİKARDİ\n----------\nTime:		Taşikardi Value:"<<endl;
	tasi>>t>>v>>v;
	while(tasi>>t>>v)
		merge<<t<<"			"<<v<<endl;
	merge<<"\n\n\n\n";
}

int main()
{
	vector<dd> rt1, rt2, rt3;
	
	ifstream data1("data08.txt",ios::in);
	ifstream data2("data16.txt",ios::in);
	ifstream data3("data23.txt",ios::in);
	
	if (!data1 || !data2 || !data3) {
		cerr << "Error opening DATA file!" << endl;
		return EXIT_FAILURE;
	}
	
	rHesaplama(data1,rt1);
	rHesaplama(data2,rt2);
	rHesaplama(data3,rt3);
	
	BPMhesapla(rt1,"Patient");
	BPMhesapla(rt2,"Patient");
	BPMhesapla(rt3,"Patient");
	
	merge("Patient");

	cout<<"Operation Complated Successfuly..."<<endl;
	
	return 0;
}
