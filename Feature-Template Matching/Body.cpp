//this is currently not being used
//could be helpful if we decide to calculate arm and gun measurements based on a person's height or width in frame

#include "Background.cpp"

using std::vector;

class BodyDetector {
    public:
    
int scanForHeight(vector< vector<int>> pic) {
	vector< vector<int> > lineSegs;
	int currStreak = 0, forgiveFactor = 10, streakFactor = 35, errStreak = 0, lineStart = -1, xStart = 0, oneWay = 1, otherWay = -1;
	int threshold = pic[0].size()/12;
	if(pic.size() > pic[0].size()) threshold = pic.size()/12;
	
	for(int xx = 0; xx<pic[0].size(); xx+=20) {
		int x = xx;
		for(int y = 0; y<pic.size(); y++) {
			if(pic[y][x] == 1) {
				currStreak++;
				errStreak--;
				if(errStreak < 0) errStreak = 0;
				if(currStreak > streakFactor && lineStart == -1) {
					lineStart = y-streakFactor;
					xStart = x;
				}
			}
			else if(x+oneWay >= 0 && x+oneWay < pic[0].size() && pic[y][x+oneWay] == 1) {
				x += oneWay;
				currStreak++;
				errStreak--;
				if(errStreak < 0) errStreak = 0;
				if(currStreak > streakFactor && lineStart == -1) lineStart = y-streakFactor;
				oneWay = oneWay * -1;
				otherWay = otherWay * -1;
			}
			else if(x+otherWay >= 0 && x+otherWay < pic[0].size() && pic[y][x+otherWay] == 1) {
				x += otherWay;
				currStreak++;
				errStreak--;
				if(errStreak < 0) errStreak = 0;
				if(currStreak > streakFactor && lineStart == -1) lineStart = y-streakFactor;
				oneWay = oneWay * -1;
				otherWay = otherWay * -1;
			}
			else {
				errStreak++;
				if(errStreak > forgiveFactor) {
					errStreak = 0;
					currStreak = 0;
					int dist = (y-forgiveFactor)-lineStart;
					if(lineStart != -1 && dist > threshold) {
						vector<int> line;
						line.push_back(xStart);
						line.push_back(lineStart);
						line.push_back(x);
						line.push_back(y-forgiveFactor);
						lineSegs.push_back(line);
					}
				}
			}
		}
		lineStart = -1;
		currStreak = 0;
		errStreak = 0;
	}
	if(lineSegs.size() == 0) return 0;
	int max = 0;
	for(int i = 0; i<lineSegs.size(); i++) {
		int dist = lineSegs[i][3] - lineSegs[i][1];
		if(dist > max) {
			max = dist;
		}
	}
	return max;
	
}
    
};
