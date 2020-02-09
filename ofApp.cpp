#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(255);
	ofSetColor(0);

	this->shader.load("shader/shader.vert", "shader/shader.frag");
	this->fbo.allocate(ofGetWidth(), ofGetHeight());
}
//--------------------------------------------------------------
void ofApp::update() {

	this->fbo.begin();
	ofClear(0);

	vector<glm::vec2> right, left;
	for (auto param = 0; param <= 50; param++) {

		auto noise_value = ofNoise(param * 0.08, ofGetFrameNum() * 0.005);

		if (noise_value < 0.65) {

			right.push_back(this->make_point(720, param) + glm::vec2(ofGetWidth() * 0.5, ofGetHeight() * 0.5));
			left.push_back(this->make_point(720, 100 - param) + glm::vec2(ofGetWidth() * 0.5, ofGetHeight() * 0.5));
		}
		else {

			if (right.size() > 0 && left.size() > 0) {

				reverse(left.begin(), left.end());
				ofBeginShape();
				ofVertices(right);
				ofVertices(left);
				ofEndShape(true);

				right.clear();
				left.clear();
			}
		}
	}

	if (right.size() > 0 && left.size() > 0) {

		reverse(left.begin(), left.end());
		ofBeginShape();
		ofVertices(right);
		ofVertices(left);
		ofEndShape(true);
	}

	this->fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw() {

	vector<glm::vec2> targets;
	for (int i = 0; i < 100; i++) {

		float R = 180 + 90 * abs(sin((i * 100 + ofGetFrameNum() + i) * DEG_TO_RAD));
		float x = R * cos((360 * i / 100 + ofGetFrameNum() * 0.15) * DEG_TO_RAD);
		float y = R * sin((360 * i / 100 + ofGetFrameNum() * 0.15) * DEG_TO_RAD);

		targets.push_back(glm::vec2(x, y) + glm::vec2(ofGetWidth() * 0.5, ofGetHeight() * 0.5));
	}

	this->shader.begin();
	this->shader.setUniform1f("time", ofGetElapsedTimef());
	this->shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	this->shader.setUniform2fv("targets", &targets[0].x, targets.size());
	this->shader.setUniformTexture("tex", this->fbo.getTexture(), 1);

	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	this->shader.end();
}

//--------------------------------------------------------------
glm::vec2 ofApp::make_point(int len, int param) {

	param = param % 100;
	if (param < 25) {

		return glm::vec2(ofMap(param, 0, 25, -len * 0.5, len * 0.5), -len * 0.5);
	}
	else if (param < 50) {

		return glm::vec2(len * 0.5, ofMap(param, 25, 50, -len * 0.5, len * 0.5));
	}
	else if (param < 75) {

		return glm::vec2(ofMap(param, 50, 75, len * 0.5, -len * 0.5), len * 0.5);
	}
	else {

		return glm::vec2(-len * 0.5, ofMap(param, 75, 100, len * 0.5, -len * 0.5));
	}
}

//--------------------------------------------------------------
int main() {

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.setSize(720, 720);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}