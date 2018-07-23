#include "IKEngine.h"



// ===============================================================================
// ================== Initializations ============================================
// ===============================================================================

// Constaructor
IKEngine::IKEngine()
{
	dir = 0;
	rotation = 0;
	walk_state = 0;
	mv_cycle = 0;
	m_div = 30;
	//walk_mode = 0;
	W1_CoordsMatrix = new double*[12];
	for (int i =0; i<12; i++){
		W1_CoordsMatrix[i] = new double[12];
			for (int j =0; j<12; j++){
				W1_CoordsMatrix[i][j] = C_W1_CoordsMatrix[i][j];
		}
	}
	// Init coords
	RFx = 12;
	RFy = -3;
	RFz = 12;

	LFx = 12;
	LFy = -3;
	LFz = 12;

	RBx = 12;
	RBy = -3;
	RBz = 12;

	LBx = 12;
	LBy = -3;
	LBz = 12;
}

// Attach all servos
void IKEngine::setPins(int RF1, int RF2, int RF3,	int LF1, int LF2, int LF3,	int RB1, int RB2, int RB3,	int LB1, int LB2, int LB3)
{
	S_RF1.attach(RF1);
	S_RF2.attach(RF2);
	S_RF3.attach(RF3);

	S_LF1.attach(LF1);
	S_LF2.attach(LF2);
	S_LF3.attach(LF3);

	S_RB1.attach(RB1);
	S_RB2.attach(RB2);
	S_RB3.attach(RB3);

	S_LB1.attach(LB1);
	S_LB2.attach(LB2);
	S_LB3.attach(LB3);
}

// Set distances of the robot legs
void IKEngine::setDist(double pcoxa, double pfemur, double ptibia)
{
	coxa=pcoxa;
	femur=pfemur;
	tibia=ptibia;
}

// Set maximums and minimums x and y of the legs (z limit = x)
void IKEngine::setLimits(double max_x, double min_x, double max_y, double min_y)
{
		Max_x = max_x;
		Min_x = min_x;
		Max_y = max_y;
		Min_y = min_y;
}

// Set angle offset of each servo
void IKEngine::setAngleOffsets(double RF1, double RF2, double RF3, double LF1, double LF2, double LF3, double RB3, double RB2, double RB1, double LB1, double LB2, double LB3)
{
	offset1RF=RF1;
	offset2RF=RF2;
	offset3RF=RF3;
	offset1LF=LF1;
	offset2LF=LF2;
	offset3LF=LF3;
	offset1LB=LB1;
	offset2LB=LB2;
	offset3LB=LB3;
	offset1RB=RB1;
	offset2RB=RB2;
	offset3RB=RB3;
}


// ===============================================================================
// ================== Setters ====================================================
// ===============================================================================

void IKEngine::setMode(int mode)
{
	//walk_mode = mode;
}

void IKEngine::setDiv(double pdiv)
{
	m_div = pdiv;
}

void IKEngine::setDir(int pdir)
{
	if (dir != pdir){
		dir = pdir;
		walk_state = 0;
		mv_cycle = 0;
	}
}

void IKEngine::setRotation(int deg)
{
	rotation = deg;
}


// ===============================================================================
// ================== Public Methods =============================================
// ===============================================================================
void IKEngine::init()
{
  writeServos();
}

// Calculates the new position of the legs
void IKEngine::updateIK()
{

	switch(dir)
	{
		case 1: // Forward
			if (movLegs(W1_CoordsMatrix, walk_state) == 1)
			{
				if (walk_state<11) walk_state++;
				else walk_state = 0;
			}

		}
}
	/*
	switch(dir){

		case 1:	// Forward
			switch(walk_state){
				case 0: 			// -- Move up and forward LB --
					if(walk_cycle < Movm_div || LBy > min_y){

						if(LBy < max_y && LBz > ((min_x-max_x)/4)+min_x){	// Up
							LBy += inc_y;
						}else if(LBy > min_y){		// Down
							LBy -= inc_y;
						}

						if (LBy > min_y){
							LBz -= inc_x;
							LBx = (max_x-min_x)/2+min_x;
						}

						if (RFz > min_x) RFz -= inc_x/4;
						if (LFz > min_x) LFz -= inc_x/4;
						if (RBz < max_x) RBz += inc_x/4;

						Movm_div++;
					}else{
						Movm_div = 0;
						walk_state++;
					}

					break;

				case 1: 			// -- Move up and forward LF --
					if(walk_cycle < Movm_div || LFy > min_y){

						if(LFy < max_y && LFz < (1/4)*(min_x-max_x)+min_x){	// Up
							LFy += inc_y;
						}else if(LFy > min_y){		// Down
							LFy -= inc_y;
						}

						if (LFy > min_y){
							LFz += inc_x;
							LFx = (max_x-min_x)/2+min_x;
						}

						if (RFz > min_x) RFz -= inc_x/4;
						if (LBz < min_x) LBz += inc_x/4;
						if (RBz < max_x) RBz += inc_x/4;

						Movm_div++;
					}else{
						Movm_div = 0;
						walk_state++;
					}

					break;

				case 2: 			// -- Move up and forward RB --
					if(walk_cycle < Movm_div || RBy > min_y){

						if(RBy < max_y && RBz > ((min_x-max_x)/4)+min_x){	// Up
							RBy += inc_y;
						}else if(RBy > min_y){		// Down
							RBy -= inc_y;
						}

						if (RBy > min_y){
							RBz -= inc_x;
							RBx = (max_x-min_x)/2+min_x;
						}

						if (RFz > min_x) RFz -= inc_x/4;
						if (LFz > min_x) LFz -= inc_x/4;
						if (LBz < max_x) LBz += inc_x/4;

						Movm_div++;
					}else{
						Movm_div = 0;
						walk_state++;
					}

					break;

				case 3: 			// -- Move up and forward RF --
					if(walk_cycle < Movm_div || RFy > min_y){

						if(RFy < max_y && RFz < (1/4)*(min_x-max_x)+min_x){	// Up
							RFy += inc_y;
						}else if(RFy > min_y){		// Down
							RFy -= inc_y;
						}

						if (RFy > min_y){
							RFz += inc_x;
							RFx = (max_x-min_x)/2+min_x;
						}

						if (LFz > min_x) LFz -= inc_x/4;
						if (LBz < min_x) LBz += inc_x/4;
						if (RBz < max_x) RBz += inc_x/4;

						Movm_div++;
					}else{
						Movm_div = 0;
						walk_state = 0;
					}
					break;

				default:
					walk_state = 0;

			}

			default:
				break;


			break;

	}
}*/

// Move all servos to the calculated position
void IKEngine::move()
{
	writeServos();
}

// Stop the movement
void IKEngine::stop()
{
	dir = 0;
	rotation = 0;
}





// ===============================================================================
// ================== Private Methods =============================================
// ===============================================================================
void IKEngine::standUp()
{
	// Init coords
	RFx = 12;
	RFy = 11;
	RFz = 12;

	LFx = 12;
	LFy = 11;
	LFz = 12;

	RBx = 12;
	RBy = 11;
	RBz = 12;

	LBx = 12;
	LBy = 11;
	LBz = 12;

	writeServos();
}

// Linearly move all legs to the given coordinates
int IKEngine::movLegs(double ** coordsMatrix, int index){

	// Objective coords
	double t_RFx = coordsMatrix[index][0];
	double t_RFy = coordsMatrix[index][1];
	double t_RFz = coordsMatrix[index][2];
	double t_RBx = coordsMatrix[index][3];
	double t_RBy = coordsMatrix[index][4];
	double t_RBz = coordsMatrix[index][5];
	double t_LFx = coordsMatrix[index][6];
	double t_LFy = coordsMatrix[index][7];
	double t_LFz = coordsMatrix[index][8];
	double t_LBx = coordsMatrix[index][9];
	double t_LBy = coordsMatrix[index][10];
	double t_LBz = coordsMatrix[index][11];


	// The first call (mv_cycle == 0) calculate the Increment to be added to 
	// the current position, to reach the destination, in "m_div" movements
	if (mv_cycle == 0){
		mv_inc[0] = (t_RFx - RFx)/double(m_div);
		mv_inc[1] = (t_RFy - RFy)/double(m_div);
		mv_inc[2] = (t_RFz - RFz)/double(m_div);
		mv_inc[3] = (t_RBx - RBx)/double(m_div);
		mv_inc[4] = (t_RBy - RBy)/double(m_div);
		mv_inc[5] = (t_RBz - RBz)/double(m_div);
		mv_inc[6] = (t_LFx - LFx)/double(m_div);
		mv_inc[7] = (t_LFy - LFy)/double(m_div);
		mv_inc[8] = (t_LFz - LFz)/double(m_div);
		mv_inc[9] = (t_LBx - LBx)/double(m_div);
		mv_inc[10] = (t_LBy - LBy)/double(m_div);
		mv_inc[11] = (t_LBz - LBz)/double(m_div);
	}

	// Increment the current position
    RFx += mv_inc[0];
    RFy += mv_inc[1];
    RFz += mv_inc[2];
    RBx += mv_inc[3];
    RBy += mv_inc[4];
    RBz += mv_inc[5];
    LFx += mv_inc[6];
    LFy += mv_inc[7];
    LFz += mv_inc[8];
    LBx += mv_inc[9];
    LBy += mv_inc[10];
    LBz += mv_inc[11];

    //writeServos();

	if (mv_cycle < m_div){
		mv_cycle++;
		return 0;
	} else return 1;
}


// Write the angles to the servos
void IKEngine::writeServos()
{
	// Safe servo angle limits
	int r2 = 25;
	int l2 = 135;
    // Set the servos to their coords with IK functions
	S_RF1.write(180 - (offset1RF + IKang1(RFx,RFy,RFz)));
    S_RF2.write(max(r2, 90 + offset2RF - IKang2(RFx,RFy,RFz)));
    S_RF3.write((180+offset3RF) - IKang3(RFx,RFy,RFz));

    S_LF1.write(IKang1(LFx,LFy,LFz) + offset1LF);
    S_LF2.write(min(l2, IKang2(LFx,LFy,LFz) + offset2LF - 90));
    S_LF3.write(offset3LF + (IKang3(LFx,LFy,LFz)-180));

    S_RB1.write(IKang1(RBx,RBy,RBz) + offset1RB);
    S_RB2.write(min(l2, IKang2(RBx,RBy,RBz) + offset2RB - 90));
    S_RB3.write(offset3RB + (IKang3(RBx,RBy,RBz) - 180));

    S_LB1.write(180 - (offset1LB + IKang1(LBx,LBy,LBz)));
    S_LB2.write(max(r2, 90 + offset2LB - IKang2(LBx,LBy,LBz)));
    S_LB3.write((180+offset3LB) - IKang3(LBx,LBy,LBz));
}



// Functions to calculate the servo angles with IK
// Angle for servo 1
int IKEngine::IKang1(double x, double y, double z){
  double gamma = atan2(z, x);
  return (gamma * TODEG);
}

// Angle for servo 2
int IKEngine::IKang2(double x, double y, double z){
  double h = sqrt((x*x) + (z*z)) - coxa;
  double a0 = atan2(h,y);
  double a1 = acos( ((femur*femur) + (h*h) + (y*y) - (tibia*tibia)) / (2*femur*sqrt((h*h) + (y*y))));

  double alpha = a0+a1;

  return (alpha * TODEG);
}

// Angle for servo 3
int IKEngine::IKang3(double x, double y, double z){
  double h = sqrt((x*x) + (z*z)) - coxa;
  double beta = acos(( (tibia*tibia) + (femur*femur) - (y*y) - (h*h) ) / (2*femur*tibia));
  return (beta * TODEG);
}
