//
// Created by niels on 02.06.15.
//

#include "Obstracle.h"
#include "Gamefield.h"
#include "Ball.h"

Obstracle::Obstracle(GamefieldPtr mGamefield, uint64_t mId, const Vector& mPosition) :
		Element(mGamefield, mId, mPosition, mGamefield->getOptions().obstracle.color,
				mGamefield->getOptions().obstracle.size) {
}

bool Obstracle::tryEat(ElementPtr other) {
	if (other->getType() == ET_Ball && other->getMass() > mGamefield->getOptions().obstracle.needMass) {
		BallPtr ball = std::dynamic_pointer_cast<Ball>(other);
		int32_t mass = ball->getMass() / 2;
		int splitcount = 10;
		if (mass < 200)
			splitcount = mass / 20;
		int32_t newmass = mass / splitcount;
		for (double angle = 0; angle < 2 * M_PI; angle += (2 * M_PI) / splitcount) {
			BallPtr b = ball->splitUp(Vector::FromAngle(angle));
			b->setMass(newmass);
		}
		mGamefield->destroyElement(ball);
		mGamefield->destroyElement(this);
		return true;
	} else if (other->getType() == ET_Shoot) {
		mEatCount++;
		if (mEatCount >= mGamefield->getOptions().obstracle.eatCount) {
			//shoot out new Obstracle
			mEatCount = 0;
		}
		//Grow up to a size of 150%
		setSize(mGamefield->getOptions().obstracle.size +
				mGamefield->getOptions().obstracle.size * 0.5 / (mGamefield->getOptions().obstracle.eatCount - 1) *
				mEatCount);
		mGamefield->destroyElement(other);
		return true;
	}
	return false;
}
