/*
Bullet Continuous Collision Detection and Physics Library
btConeTwistConstraint is Copyright (c) 2007 Starbreeze Studios

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Written by: Marcus Hennix
*/



#ifndef CONETWISTCONSTRAINT_H
#define CONETWISTCONSTRAINT_H

#include "LinearMath/btVector3.h"
#include "btJacobianEntry.h"
#include "btTypedConstraint.h"

class btRigidBody;


///btConeTwistConstraint can be used to simulate ragdoll joints (upper arm, leg etc)
class btConeTwistConstraint : public btTypedConstraint
{
#ifdef IN_PARALLELL_SOLVER
public:
#endif
	btJacobianEntry	m_jac[3]; //3 orthogonal linear constraints

	btTransform m_rbAFrame; 
	btTransform m_rbBFrame;

	btScalar	m_limitSoftness;
	btScalar	m_biasFactor;
	btScalar	m_relaxationFactor;

	btScalar	m_damping;

	btScalar	m_swingSpan1;
	btScalar	m_swingSpan2;
	btScalar	m_twistSpan;

	btScalar	m_fixThresh;

	btVector3   m_swingAxis;
	btVector3	m_twistAxis;

	btScalar	m_kSwing;
	btScalar	m_kTwist;

	btScalar	m_twistLimitSign;
	btScalar	m_swingCorrection;
	btScalar	m_twistCorrection;

	btScalar	m_twistAngle;

	btScalar	m_accSwingLimitImpulse;
	btScalar	m_accTwistLimitImpulse;

	bool		m_angularOnly;
	bool		m_solveTwistLimit;
	bool		m_solveSwingLimit;

	bool	m_useSolveConstraintObsolete;

	// not yet used...
	btScalar	m_swingLimitRatio;
	btScalar	m_twistLimitRatio;
	btVector3   m_twistAxisA;

	// motor
	bool		 m_bMotorEnabled;
	bool		 m_bNormalizedMotorStrength;
	btQuaternion m_qTarget;
	btScalar	 m_maxMotorImpulse;
	btVector3	 m_accMotorImpulse;
	
public:

	btConeTwistConstraint(btRigidBody& rbA,btRigidBody& rbB,const btTransform& rbAFrame, const btTransform& rbBFrame);
	
	btConeTwistConstraint(btRigidBody& rbA,const btTransform& rbAFrame);

	btConeTwistConstraint();

	virtual void	buildJacobian();

	virtual void getInfo1 (btConstraintInfo1* info);
	
	virtual void getInfo2 (btConstraintInfo2* info);
	

	virtual	void	solveConstraintObsolete(btSolverBody& bodyA,btSolverBody& bodyB,btScalar	timeStep);

	void	updateRHS(btScalar	timeStep);

	const btRigidBody& getRigidBodyA() const
	{
		return m_rbA;
	}
	const btRigidBody& getRigidBodyB() const
	{
		return m_rbB;
	}

	void	setAngularOnly(bool angularOnly)
	{
		m_angularOnly = angularOnly;
	}

	void	setLimit(int limitIndex,btScalar limitValue)
	{
		switch (limitIndex)
		{
		case 3:
			{
				m_swingSpan1 = limitValue;
				break;
			}
		case 4:
			{
				m_swingSpan2 = limitValue;
				break;
			}
		case 5:
			{
				m_twistSpan = limitValue;
				break;
			}
		default:
			{
			}
		};
	}

	void	setLimit(btScalar _swingSpan1,btScalar _swingSpan2,btScalar _twistSpan,  btScalar _softness = 0.8f, btScalar _biasFactor = 0.3f, btScalar _relaxationFactor = 1.0f)
	{
		m_swingSpan1 = _swingSpan1;
		m_swingSpan2 = _swingSpan2;
		m_twistSpan  = _twistSpan;

		m_limitSoftness =  _softness;
		m_biasFactor = _biasFactor;
		m_relaxationFactor = _relaxationFactor;
	}

	const btTransform& getAFrame() { return m_rbAFrame; };	
	const btTransform& getBFrame() { return m_rbBFrame; };

	inline int getSolveTwistLimit()
	{
		return m_solveTwistLimit;
	}

	inline int getSolveSwingLimit()
	{
		return m_solveTwistLimit;
	}

	inline btScalar getTwistLimitSign()
	{
		return m_twistLimitSign;
	}

	void calcAngleInfo();
	void calcAngleInfo2();

	inline btScalar getSwingSpan1()
	{
		return m_swingSpan1;
	}
	inline btScalar getSwingSpan2()
	{
		return m_swingSpan2;
	}
	inline btScalar getTwistSpan()
	{
		return m_twistSpan;
	}
	inline btScalar getTwistAngle()
	{
		return m_twistAngle;
	}
	bool isPastSwingLimit() { return m_solveSwingLimit; }


	void setDamping(btScalar damping) { m_damping = damping; }

	void enableMotor(bool b) { m_bMotorEnabled = b; }
	void setMaxMotorImpulse(btScalar maxMotorImpulse) { m_maxMotorImpulse = maxMotorImpulse; m_bNormalizedMotorStrength = false; }
	void setMaxMotorImpulseNormalized(btScalar maxMotorImpulse) { m_maxMotorImpulse = maxMotorImpulse; m_bNormalizedMotorStrength = true; }

	btScalar getFixThresh() { return m_fixThresh; }
	void setFixThresh(btScalar fixThresh) { m_fixThresh = fixThresh; }

	// setMotorTarget:
	// q: the desired rotation of bodyA wrt bodyB.
	// note: if q violates the joint limits, the internal target is clamped to avoid conflicting impulses (very bad for stability)
	// note: don't forget to enableMotor()
	void setMotorTarget(const btQuaternion &q);

	// same as above, but q is the desired rotation of frameA wrt frameB in constraint space
	void setMotorTargetInConstraintSpace(const btQuaternion &q);

	btVector3 GetPointForAngle(btScalar fAngleInRadians, btScalar fLength) const;



protected:
	void init();

	void computeConeLimitInfo(const btQuaternion& qCone, // in
		btScalar& swingAngle, btVector3& vSwingAxis, btScalar& swingLimit); // all outs

	void computeTwistLimitInfo(const btQuaternion& qTwist, // in
		btScalar& twistAngle, btVector3& vTwistAxis); // all outs

	void adjustSwingAxisToUseEllipseNormal(btVector3& vSwingAxis) const;
};

#endif //CONETWISTCONSTRAINT_H
