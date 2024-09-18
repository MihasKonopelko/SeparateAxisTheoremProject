#include "stdafx.h" 
#include "CollisionDetection.h"

CollisionDetection * CollisionDetection::m_instance = 0;


CollisionDetection::CollisionDetection(int borderWidth, int borderHeight):
	m_borderMax(borderWidth,borderHeight){}

void CollisionDetection::BorderCollision(BouncingThing* obj)
{
	if (obj->getPosition().y > m_borderMax.y)
		obj->setVelocity(sf::Vector2f(obj->getVelocity().x,obj->getVelocity().y*-1));
	
	if (obj->getPosition().y < 0)
		obj->setVelocity(sf::Vector2f(obj->getVelocity().x,obj->getVelocity().y*-1));
	
	if (obj->getPosition().x >  m_borderMax.x)
		obj->setVelocity(sf::Vector2f(obj->getVelocity().x*-1,obj->getVelocity().y));
	
	if (obj->getPosition().x < 0)
		obj->setVelocity(sf::Vector2f(obj->getVelocity().x*-1,obj->getVelocity().y));
	
}

CollisionDetection * CollisionDetection::instance(int borderWidth, int borderHeight) 
{
	if (m_instance == 0)  
		m_instance = new CollisionDetection(borderWidth, borderHeight);
	
    return m_instance;
}


bool CollisionDetection::CheckSA(std::vector<sf::Vector2f> sa,BouncingThing* obj1,BouncingThing* obj2)
{
	float minA = 0 ,maxA = 0;    
	float minB = 0, maxB = 0;

	std::vector<sf::Vector2f> m_obj1Verts = obj1->getVerteciesScreenPosition();
	std::vector<sf::Vector2f> m_obj2Verts = obj2->getVerteciesScreenPosition();;

	std::vector<sf::Vector2f>::iterator saItt = sa.begin();

	for (; saItt < sa.end(); ++saItt)
	{
		std::vector<sf::Vector2f>::iterator objVPItt = m_obj1Verts.begin();

		for (int i =0; objVPItt < m_obj1Verts.end(); ++objVPItt,i++)    
		{  
			float res =objVPItt->x * saItt->x + objVPItt->y * saItt->y;
  
			if (i == 0) maxA =  minA =  res;
                 
			if (res > maxA) maxA = res;
                  
			if (res < minA) minA = res;
		}

		// Offset
		minA += obj1->getPosition().x * saItt->x +obj1->getPosition().y * saItt->y;
		maxA += obj1->getPosition().x * saItt->x +obj1->getPosition().y * saItt->y;

   
		objVPItt = m_obj2Verts.begin();


		for (int i =0; objVPItt < m_obj2Verts.end(); ++objVPItt,i++)    
		{  
			float res =objVPItt->x * saItt->x + objVPItt->y * saItt->y;
  
			if (i == 0) maxB = minB = res;
			 
			if (res > maxB) maxB =res;
                      
			if (res < minB) minB = res;
		}

		// Offset
		minB += obj2->getPosition().x * saItt->x +obj2->getPosition().y * saItt->y;
		maxB += obj2->getPosition().x * saItt->x +obj2->getPosition().y * saItt->y;   

		if ( minA>maxB || minB>maxA)    
			return true;	        
	}        
	return false;
}

bool CollisionDetection::CheckForCollisionSAT(BouncingThing* obj1,BouncingThing* obj2)
{
	std::vector<sf::Vector2f> obj1SA = obj1->getSA();
	std::vector<sf::Vector2f> obj2SA = obj2->getSA();
  
	if (CheckSA(obj1SA,obj1,obj2))      
		return false;
            
	else if(CheckSA(obj2SA,obj1,obj2)) 
		return false;
          
	else      
		return true;
}

bool CollisionDetection::CollisionCheck(BouncingThing* obj1,BouncingThing* obj2)
{	
	// Pair redundancy check
	if (!obj1->CheckIfCollidedAlready(obj2->getId())|| !obj2->CheckIfCollidedAlready(obj1->getId()))
	{
		// Bound Circle collision check
		if (sqrt(	pow((obj1->getPosition().x -obj2->getPosition().x),2)+
					pow((obj1->getPosition().y -obj2->getPosition().y),2))   < obj1->getBoundigCircleRadius()+ obj2->getBoundigCircleRadius())
				{
					if (CheckForCollisionSAT( obj1, obj2))
					{
						// Responce

						sf::Vector2f temp = obj1->getVelocity();
						obj1->setVelocity(obj2->getVelocity());
						obj2->setVelocity(temp);

						obj1->AddCollidePair(obj2->getId());
						obj2->AddCollidePair(obj1->getId());

						return true;
					}
		obj1->AddCollidePair(obj2->getId());
		obj2->AddCollidePair(obj1->getId());
		}

	}
	return false;
}





