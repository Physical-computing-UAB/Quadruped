#include "IKEngine.h"



// ===============================================================================
// ================== Initializations ============================================
// ===============================================================================

// Constaructor
IKEngine::IKEngine()
{
	// TODO
}

// Attach all servos
void IKEngine::setPins(int RF1, int RF2, int RF3,	int LF1, int LF2, int LF3,	int RB1, int RB2, int RB3,	int LB1, int LB2, int LB3)
{
	// TODO
}
		
// Set distances of the robot legs
void IKEngine::setDist(float coxa, float femur, float tibia)
{
	// TODO
}

// Set maximums and minimums x and y of the legs (z limit = x)
void IKEngine::setLimits(double Max_x, double Min_x, double Max_y, double Min_y)
{
		// TODO
}
		
// Set angle offset of each servo
void IKEngine::setAngleOffsets(double RF1, double RF2, double RF3, double LF1, double LF2, double LF3, double RB3, double RB2, double RB1, double LB1, double LB2, double LB3)
{
	// TODO
}


// ===============================================================================
// ================== Setters ====================================================
// ===============================================================================

void IKEngine::setMode(int mode)
{
	walk_mode = mode;
}

void IKEngine::setSpeed(float sp)
{
	MovDiv = sp;
	inc_x = (max_x-min_x)/sp;
	inc_y = (max_y-min_y)/(sp/4);
}

void IKEngine::setDir(float dir)
{
	dir = dir;
}

void IKEngine::setRotation(float deg)
{
	rotation = deg;
}

void IKEngine::stopAll()
{
	dir = 0;
	rotation = 0;
}


// ===============================================================================
// ================== Public Methods =============================================
// ===============================================================================

// Calculates the new position of the legs
void IKEngine::updateIK()
{
	switch(dir){
		
		case 1:	// Forward
			switch(walk_state){
				case 0: 			// -- Move up and forward LB --
					if(walk_cycle < MovDiv || LBy > min_y){
						
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
						
						MovDiv++;
					}else{
						MovDiv = 0;
						walk_state++;
					}
					
					break;
				
				case 1: 			// -- Move up and forward LF --
					if(walk_cycle < MovDiv || LFy > min_y){
						
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
						
						MovDiv++;
					}else{
						MovDiv = 0;
						walk_state++;
					}
					
					break;
					
				case 2: 			// -- Move up and forward RB --
					if(walk_cycle < MovDiv || RBy > min_y){
						
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
						
						MovDiv++;
					}else{
						MovDiv = 0;
						walk_state++;
					}
					
					break;
					
				case 3: 			// -- Move up and forward RF --
					if(walk_cycle < MovDiv || RFy > min_y){
						
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
						
						MovDiv++;
					}else{
						MovDiv = 0;
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
}
		
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


// Linearly move all legs to the given coordinates
void IKEngine::movLegs(double t_RFx, double t_RFy, double t_RFz, double t_RBx, double t_RBy, double t_RBz, double t_LFx, double t_LFy, double t_LFz, double t_LBx, double t_LBy, double t_LBz, int div, int time){
  
  // div -> divisions (number of movements to reach the new coordinates)
  // time-> time between each sub-movement

  double subxRF = (t_RFx - RFx)/div;  // Increment to be added to the current position to reach the destination in "div" movements
  double subyRF = (t_RFy - RFy)/div;
  double subzRF = (t_RFz - RFz)/div;
  double subxRB = (t_RBx - RBx)/div;
  double subyRB = (t_RBy - RBy)/div;
  double subzRB = (t_RBz - RBz)/div;
  double subxLF = (t_LFx - LFx)/div;
  double subyLF = (t_LFy - LFy)/div;
  double subzLF = (t_LFz - LFz)/div;
  double subxLB = (t_LBx - LBx)/div;
  double subyLB = (t_LBy - LBy)/div;
  double subzLB = (t_LBz - LBz)/div;

  // Do the "div" movements
  for(int i = 0; i < div; i++){
    RFx += subxRF;
    RFy += subyRF;
    RFz += subzRF;
    RBx += subxRB;
    RBy += subyRB;
    RBz += subzRB;
    LFx += subxLF;
    LFy += subyLF;
    LFz += subzLF;
    LBx += subxLB;
    LBy += subyLB;
    LBz += subzLB;

    writeServos();
    
    delay(time);
  } 
}


// ===============================================================================
// ================== Private Methods =============================================
// ===============================================================================

// Calculates the legs coordinates
void IKEngine::walk()
{
	// TODO
}

// Write the angles to the servos
void IKEngine::writeServos()
{
	S_RF1.write(offset1RF - IKang1(RFx,RFy,RFz));
	S_RF2.write(offset2RF - IKang2(RFx,RFy,RFz));
	S_RF3.write(IKang3(RFx,RFy,RFz) - offset3RF);
	
	S_LF1.write(IKang1(LFx,LFy,LFz) + offset1LF);
	S_LF2.write(IKang2(LFx,LFy,LFz) - offset2LF);
	S_LF3.write(offset3LF - IKang3(LFx,LFy,LFz));
	
	S_RB1.write(IKang1(RBx,RBy,RBz) + offset1RB );
	S_RB2.write(IKang2(RBx,RBy,RBz) - offset2RB);
	S_RB3.write(offset3RB - IKang3(RBx,RBy,RBz));

	S_LB1.write(offset1LB - IKang1(LBx,LBy,LBz));
	S_LB2.write(offset2LB - IKang2(LBx,LBy,LBz));
	S_LB3.write(IKang3(LBx,LBy,LBz) - offset3LB);
}

// Functions to calculate the servo angles with IK
int IKEngine::IKang1(double x, double y, double z)
{
	double gamma = atan2(z, x);
	return (gamma * TODEG);
}
int IKEngine::IKang2(double x, double y, double z)
{
	  double hip = sqrt( pow(y,2) + pow((x - coxa),2));
	  double alpha1 = acos(y/hip);
	  double alpha2 = acos( (pow(tibia,2) - pow(femur, 2) - pow(hip,2))/(-2*femur*hip));
	  double alpha = alpha1 + alpha2;
	  return (alpha * TODEG);
}
int IKEngine::IKang3(double x, double y, double z)
{
	  double hip = sqrt( pow(y,2) + pow((x - coxa),2));
	  double beta = acos(( pow(hip,2) - pow(tibia,2) - pow(femur,2))/(-2*tibia*femur));
	  return (beta * TODEG);
}
