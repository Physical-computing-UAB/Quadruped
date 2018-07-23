#ifndef IKEngine_h
#define IKEngine_h

// Include libraries.
#include <Arduino.h>
#include <Servo.h>

class IKEngine {

	public:

		static const double TODEG = 57.3; // Conversion from rad to deg

		IKEngine();

		// ================== Initializations ==================
		void init();

		// Attach all servos
		void setPins(int RF1, int RF2, int RF3,
					int LF1, int LF2, int LF3,
					int RB1, int RB2, int RB3,
					int LB1, int LB2, int LB3);

		// Set distances of the robot legs
		void setDist(double pcoxa, double pfemur, double ptibia);

		// Set maximums and minimums x and y of the legs (z limit = x)
		void setLimits(double Max_x, double Min_x,
						double Max_y, double Min_y);

		// Set angle offset of each servo
		void setAngleOffsets(double RF1, double RF2, double RF3,
					double LF1, double LF2, double LF3,
					double RB1, double RB2, double RB3,
					double LB1, double LB2, double LB3);


		// ======================================================

		// ================== Setters ==================

		void setMode(int mode);

		void setDiv(double pdiv);

		void setDir(int dir);

		void setRotation(int deg);

		// ======================================================

		// ======================================================

		// Calculates the new position of the legs
		// It's called each iteration to complete the walk step by step
		void updateIK();

		// Move all servos to the calculated position
		void move();

		// Stop the movement
		void stop();

	private:

		// Distances of the leg (cm)
		double coxa;
		double femur;
		double tibia;

		// Coordinates x, y, z of each leg
		double RFx, RFy, RFz;	// Right - Front
		double LFx, LFy, LFz;	// Left  - Front
		double RBx, RBy, RBz;	// Right - Back
		double LBx, LBy, LBz;	// Left  - Back

		// Max and Mins coordinates where legs can go
		double Max_x, Min_x;
		double Max_y, Min_y;

		// Offset angles of each servo
		double offset1RF, offset2RF, offset3RF;
		double offset1LF, offset2LF, offset3LF;
		double offset1LB, offset2LB, offset3LB;
		double offset1RB, offset2RB, offset3RB;

		// Servos
		Servo S_RF1, S_RF2, S_RF3;
		Servo S_LF1, S_LF2, S_LF3;
		Servo S_RB1, S_RB2, S_RB3;
		Servo S_LB1, S_LB2, S_LB3;

		int walk_state;		// States of the walk cycle [0,1,2,3]
		int walk_cycle;		// Walk cycle number [0,...,MovDiv]
		double inc_x;		// Increment to add to the legs coordinates each iteration
		double inc_y;		// Increment to add to the legs coordinates each iteration


		// -- State variables --
		int walk_mode;	// Switch between different modes
		double m_div;	// Division of the walk cycle (Speed of the movements)
		int dir;	// 0, 1, 2, 3, 4 (stop, forward, right, left, back)
		double rotation;	// -1, 0, 1 -> rotation direction (left, none, right)


		// Functions to calculate the servo angles with IK
		int IKang1(double x, double y, double z);
		int IKang2(double x, double y, double z);
		int IKang3(double x, double y, double z);



		// -- Private methods --
		void initPositions();
		int movLegs(double ** coordsMatrix, int index);
		void standUp();
		void walk();		// Calculates the legs coordinates
		void writeServos(); 	// Write the angles to the servos

		// -- movLegs variables --
		int mv_cycle;
		double mv_inc[12];



		// Walk1 coordinates matrix
		double ** W1_CoordsMatrix;
		double C_W1_CoordsMatrix[12][12] =
		{{14,9,4.1,     14,4,16,     14,6,14.3,       14,9,5.8},
		{14,9,2.4,      14,4,-1,     14,6,12.6,       14,9,7.5},
		{14,9,0.7,      14,9,-1,     14,9,10.9,       14,9,9.2},
		{14,4,-1,       14,9,0.7,    14,9,9.2,        14,6,10.9},
		{14,4,16,       14,9,2.4,    14,9,7.5,        14,6,12.6},
		{14,9,16,       14,9,4.1,    14,9,5.8,        14,9,14.3},
		{14,6,14.3,     14,9,5.8,    14,9,4.1,        14,4,16},
		{14,6,12.6,     14,9,7.5,    14,9,2.4,        14,4,-1},
		{14,9,10.9,     14,9,9.2,    14,9,0.7,        15,9,-1},
		{14,9,9.2,      14,6,10.9,   14,4,-1,         14,9,0.7},
		{14,9,7.5,      14,6,12.6,   14,4,16,         14,9,2.4},
		{14,9,5.8,      14,9,14.3,   14,9,16,         14,9,4.1}};



};

#endif
