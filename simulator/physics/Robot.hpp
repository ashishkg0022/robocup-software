#pragma once
#include "Entity.hpp"
#include "SimEngine.hpp"
#include "RaycastVehicle.hpp"

#include <protobuf/RadioTx.pb.h>
#include <protobuf/RadioRx.pb.h>


class Ball;
class RobotBallController;
class GL_ShapeDrawer;

class Robot : public Entity
{
public:
	enum WheelIndex {
		FrontLeft  = 0,
		FrontRight = 1,
	    BackRight  = 2,
	    BackLeft   = 3
	};

	typedef enum {
		rev2008,
		rev2011
	} RobotRevision;

	// assigned shell number
	unsigned int shell;

	/**
	 * A value between 0 and 100 indicating how good visibility of the Robot is.  A value of 99 indicates that
	 * 1% of the time this robot will be omitted from the vision packet sent back to soccer.
	 */
	int visibility;


public:
	Robot(Environment* env, unsigned int id, Robot::RobotRevision rev, const Geometry2d::Point& startPos);
	~Robot();

	void initPhysics(const bool& blue);

	//the following are in field space, aka soccer's coordinate system

	// Entity interface
	virtual void position(float x, float y); // world coords

	virtual void setTargetVelocity(float x, float y, float w); // body coords

	virtual Geometry2d::Point getPosition() const;

	virtual float getAngle() const;

	// debug text interface
	Geometry2d::Point getVelFS() const;

	float getAngVelFS() const;

	Geometry2d::Point getTargetVelFS() const;

	float getTargetAngVelFS() const;


public:

	RobotRevision revision() const { return _rev; }


	btRigidBody* getRigidBody() const {
		return _robotChassis;
	}

	RaycastVehicle* getRaycastVehicle() const {
		return _robotVehicle;
	}

	RobotBallController* getRobotController() const {
		return _controller;
	}

	SimEngine* getSimEngine() {
		return _simEngine;
	}

	void getWorldTransform(btTransform& chassisWorldTrans) const;

	/**
	 * Sets the engine foce for each wheel to @val
	 * @val value to set engine forces to
	 */
	void setEngineForce(float val) {
		for (int i=0; i<4; i++) {
			_engineForce[i] = val;
		}
	}

	/** set control data */
	void radioTx(const Packet::RadioTx::Robot *data);

	/** get robot information data */
	Packet::RadioRx radioRx() const;

	void renderWheels(GL_ShapeDrawer* shapeDrawer, const btVector3& worldBoundsMin, const btVector3& worldBoundsMax) const;

	void applyEngineForces(float deltaTime);

	void resetScene();


protected:
	RobotRevision _rev;

	// physics components
	btRigidBody* _robotChassis;
	RaycastVehicle::btVehicleTuning _tuning;
	btVehicleRaycaster* _vehicleRayCaster;
	RaycastVehicle* _robotVehicle;
	btCollisionShape* _wheelShape;

	RobotBallController* _controller;

	// control inputs
	float _engineForce[4];

	btVector3 _targetVel;
	btScalar  _targetRot;

	// state info
	btTransform _startTransform;

	// links to the engine
	SimEngine *_simEngine;

	/** center of roller from ground */
	constexpr static float RollerHeight = .03;
	/** center of roller from center of robot */
	constexpr static float RollerOffset = .065;
	/** roller length */
	constexpr static float RollerLength = .07;
	/** radius of the roller */
	constexpr static float RollerRadius = .01;

	/** width of the kicker face */
	constexpr static float KickerFaceWidth = .05;
	/** height of the kicker face */
	constexpr static float KickerFaceHeight = .005;
	/** depth of the kicker plate */
	constexpr static float KickerLength = .03;
};
