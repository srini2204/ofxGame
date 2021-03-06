/*
 *  ofxElement.h
 *
 *  Created by Patricio González Vivo on 02/07/11.
 *  Copyright 2011 PatricioGonzalezVivo.com. All rights reserved.
 *
 *
 *  This work is licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 3.0 
 *	Unported License. To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/3.0/ 
 *	or send a letter to Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 *
 */

#ifndef OFXELEMENT
#define OFXELEMENT

#include "ofMain.h"

class ofxElement{
public:
	ofxElement(string _imagePath){
		iPath = _imagePath;
		iPart.loadImage(iPath);
		width = iPart.getWidth();
		height = iPart.getHeight();
		imageCenter = ofVec2f(-width*0.5,-height*0.5);
		mirror = false;
		north = 0;
	};
	
	ofxElement(string _imagePath,float _rad, float _ang, int _iX, int _iY,bool _mirror){
		north = 0;
		radio = _rad;
		angle = _ang;
		iPath = _imagePath;
		iPart.loadImage(iPath);
		mirror = _mirror;
		width = iPart.getWidth();
		height = iPart.getHeight();
		imageCenter = ofVec2f(_iX,_iY);
		//cout << "-- Loading Element " << _imagePath << " w:" << width << " h:" << height << endl;
	};
	
	ofxElement&	setScale(float * _scale){scale = _scale; return * this;};
	ofxElement&	setBodyNorth(float * _bodyNorth){bodyNorth = _bodyNorth; return * this;};
	ofxElement&	setBodyCenter(ofVec2f * _bodyCenter){bodyCenter = _bodyCenter; return * this;};
	ofxElement&	setRadio(float _radio){radio = _radio; return * this;};
	ofxElement&	setRadio(ofVec2f _pos){ radio = _pos.distance(*bodyCenter); return * this;};
	ofxElement&	setAngle(float _angle){angle = _angle; return * this;};
	ofxElement&	setAngle(ofVec2f _pos){ _pos -= *bodyCenter; angle = ( (-1*atan2(_pos.x,_pos.y)+(PI/2) - *bodyNorth)); return * this;};
	ofxElement&	moveTo(int _x, int _y){moveTo(ofVec2f(_x,_y)); return * this;};
	ofxElement&	moveTo(ofVec2f _location){ setRadio(_location); setAngle(_location); return * this;};
	
	void fromPolarToCart(){ // Transform from polar to Cartesian
		float propRadio = radio * (*scale);
		
		pos.x = propRadio * cos(angle + *bodyNorth);
		pos.y = propRadio * sin(angle + *bodyNorth);
		pos += *bodyCenter;
	}; 
	
	void	draw(){ fromPolarToCart(); draw(pos.x, pos.y,1);};
	void	draw(float _scale){ fromPolarToCart(); draw(pos.x, pos.y,_scale);};
	void	draw(int _x, int _y, float _scale){
		ofPushMatrix();
		ofTranslate(_x,_y);
		ofScale(_scale, _scale);
		ofRotateZ(ofRadToDeg(*bodyNorth + north));
		ofScale(*scale, *scale, 1);
		if (!mirror)
			iPart.draw(imageCenter.x,imageCenter.y,width,height);
		else
			iPart.draw(imageCenter.x+width,imageCenter.y,-width,height);
		
		ofPopMatrix();
	};
	
	float	getRadio(){return radio;};
	float	getWidth(){return width;};
	float	getHeight(){return height;};
	
	bool	isOver(int _x, int _y){ isOver(ofVec2f(_x,_y));};
	bool	isOver(ofVec2f _location){
		ofVec2f dirToCenter = pos - _location;
		float theta = atan2(dirToCenter.x,dirToCenter.y)-(PI/2);
		float r = dirToCenter.length();
		float x = r * cos(theta + *bodyNorth);
		float y = r * sin(theta + *bodyNorth);
		
		bool over = false;
		
		if ( (x <= width*0.5*(*scale)) && (x >= -width*0.5*(*scale)) && (y <= height*0.5*(*scale)) && (y >= -height*0.5*(*scale)))
			over = true;
		else over = false;
		
		return over;
	};
	
	ofVec2f	pos;
	float	north;
	bool	mirror;
	
protected:
	ofImage iPart;
	string	iPath;
	
	ofVec2f imageCenter;
	ofVec2f * bodyCenter;
	
	float * bodyNorth;
	float * scale;
	float	radio,angle;
	int		width,height;
};
#endif
