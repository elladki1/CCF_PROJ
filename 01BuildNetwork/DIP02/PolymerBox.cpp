/*
Generate LAMMPS input file for polymer in a box using PBCs
*/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

// Generate random number and divide by RAND_MAX (max int returned by rand()) 
//      to get random value between 0 and 1 (inclusive)
double RandVal(){
    return (rand()%RAND_MAX)/(double)RAND_MAX;
}

// Generate a random position vector bounded in x_max, y_max, z_max
void RandPosVec(std::vector<double>& pos, double x_max, double y_max, double z_max){
    pos[0]=(2*RandVal()-1.0)*x_max;
    pos[1]=(2*RandVal()-1.0)*y_max;
    pos[2]=(2*RandVal()-1.0)*z_max;
}


// Normal Gaussian, random unit vector 
void RandUnitVec(std::vector<double>& pos){
    double x,y,s;
    s=2.0; 
    while(s>1.0){
        x=2.0*RandVal() - 1.0;
        y=2.0*RandVal() - 1.0;
        s=pow(x,2)+pow(y,2);
    }
    pos[2]=1.0-2.0*s;
    s=2.0*sqrt(1.0-s);
    pos[0]=s*x;
    pos[1]=s*y;
}

int main(int argc, char** argv){
    srand(time(nullptr));
    int NN; // degree of polymerization
    int nchain; // number of chains  
    double dipole; // end charge 
    double rho = 0.005; //initial density 
    std::string buffer, TempC;
    int i,j,k;
    std::ofstream fpw0("polymer.dat");
    
    // Get user input
    while(std::getline(std::cin,buffer)){
        if(buffer.find("NN")!=std::string::npos){
            std::istringstream iss(buffer);
            iss >> TempC >> NN;  // extract the string and store in TempC and extract the integer and store in NN
            std::cout << "# " << TempC << " " << NN << std::endl;
        }
        if (buffer.find("mm") != std::string::npos) {  
            std::istringstream iss(buffer);
            iss >> TempC >> nchain;
            std::cout << "# " << TempC << " " << nchain << std::endl;
        }
        if (buffer.find("ch") != std::string::npos) {  
            std::istringstream iss(buffer);
            iss >> TempC >> dipole;
            std::cout << "# " << TempC << " " << dipole << std::endl;
        }
    }

    // Compute box size
    std::vector<double> pbcl(3);
    pbcl[0]=pow(NN*nchain/rho,1.0/3.0)/2.0;
    pbcl[1]=pbcl[0];
    pbcl[2]=pbcl[0];

    std::cout <<"# N: "<<NN<<std::endl;
    std::cout <<"# chains: "<<nchain<<std::endl;
    std::cout <<"# half box size: "<<pbcl[0]<<std::endl;

    // Allocate coordinates array 
    std::vector<std::vector<std::vector<double>>> rr(nchain,std::vector<std::vector<double>>(NN,std::vector<double>(3)));

    // assign coordinates to array 
    for(i=0; i<nchain;i++){
        // choose coodinate of first monomer
        if(i%2==0) std::cout<<"\t"<<i<<std::endl;
        std::vector<double> pos(3);
        
        RandPosVec(pos, pbcl[0]-1.0, pbcl[1]-1.0, pbcl[2]-1.0);
        rr[i][0]=pos;

        RandUnitVec(pos);
        for(j=1; j<NN; j++){
            while (true){
                int chk=0;
                RandUnitVec(pos);
                for(k=0;k<3;k++){
                    if(rr[i][j-1][k]+pos[k]>pbcl[k] || rr[i][j-1][k]+pos[k]<-pbcl[k]){
                        chk = 1;
                        break;
                    }
                }
                if(chk==0) break;
            }
           for(k=0;k<3;k++) rr[i][j][k]=rr[i][j-1][k]+pos[k]; 
            
        }
    }

    // Create LAMMPS data file
    int nAtom=nchain*NN;
    int nBond{0};
    int nAngle{0};
    if(NN>1) nBond=nchain*(NN-1);
    if(NN>2) nAngle=nchain*(NN-2);

    fpw0 << "LAMMPS Description\n\n";
    fpw0 << nAtom << " atoms \n";
    if(NN>1) fpw0 << nBond << " bonds \n";
    if(NN>2) fpw0 << nAngle << " angles \n\n";

    fpw0 << "1 atom types\n";
    if(NN>1) fpw0 << "1 bond types\n";
    if(NN>2) fpw0 << "1 angle types \n\n";

    fpw0 << -pbcl[0] << ' ' << pbcl[0] << " xlo xhi\n";
    fpw0 << -pbcl[1] << ' ' << pbcl[1] << " ylo yhi\n";
    fpw0 << -pbcl[2] << ' ' << pbcl[2] << " zlo zhi\n\n";

    fpw0 << "Masses \n\n";
    fpw0 << "1 1.00 \n\n";

    fpw0 << "Atoms\n\n";
    
    int aID = 0; // atom ID
    int mID = 0; // chain ID

    for(i=0; i<nchain; i++){
        mID++;
        for(j=0; j<NN; j++){
            aID++;
            
            if(j==0){
                fpw0 << aID << ' ' << mID << " 1 -" << dipole << " " << rr[i][j][0] << ' '
                        << rr[i][j][1] << ' ' << rr[i][j][2] << "\n";
            }
            else if(j==NN-1){
                fpw0 << aID << ' ' << mID << " 1 " << dipole << " " << rr[i][j][0] << ' '
                        << rr[i][j][1] << ' ' << rr[i][j][2] << "\n";
            }
            else{
                fpw0 << aID << ' ' << mID << " 1 0 " << rr[i][j][0] << ' '
                        << rr[i][j][1] << ' ' << rr[i][j][2] << "\n";
            }
        }
    }

    // bonds 
    int v;
    if(NN>1){
        fpw0 << "\nBonds\n\n";
        v=0;
        for(i=0; i<nchain; i++){
            for(j=0; j<NN-1; j++){
                v++;
                fpw0 << v << " 1 " << i*NN+j+1 << ' ' << i*NN+j+2 << "\n";
            }
        }
    }

    // angles
    if(NN>2){
        fpw0 << "\nAngles\n\n";
        v=0;
        for(i=0; i<nchain; i++){
            for(j=0; j<NN-2; j++){
                v++;
                fpw0 << v << " 1 " << i*NN+j+1 << ' ' << i*NN+j+2 << ' ' << i*NN+j+3 << "\n";
            }
        }
    }
    
    fpw0.close();
    return 0;
}