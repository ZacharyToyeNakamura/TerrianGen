#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>

#include "Box.cpp"

using namespace std;

class BoxGroup {
    private:
        int x;
        int y;
        int xLen;
        int yLen;
        vector<Box*> boxes;

    public:

        /**
         * Coords of boxes are ignored and set based on x, y, xlen, ylen.
        */
        BoxGroup(int x, int y, int xl, int yl, vector<Box*> boxes = {}) {
            this->x = x;
            this->y = y;
            xLen = xl;
            yLen = yl;
            this->boxes = boxes;
            for(int i = 0; i < boxes.size(); i++) {
                boxes[i]->y1 = y + yLen * i;
                boxes[i]->y2 = boxes[i]->y1 + yLen;
                boxes[i]->x1 = x;
                boxes[i]->x2 = x + xLen;
                cout << boxes[i]->x1 << " " << boxes[i]->y1 << ", " << boxes[i]->x2 << " " << boxes[i]->y2 << endl;
            }
        }

        // int getSize() {return boxes.size();}
        
        /**
         * Adds a box below the last box
         * Maybe should be a pointer
        */
        void add(Box *box) {
            box->y1 = y + yLen * boxes.size();
            box->y2 = box->y1 + yLen;
            box->x1 = x;
            box->x2 = x + xLen;
            boxes.push_back(box);
        }

        // // Erases a box at a certain index
        // int erase(int idx) {
        //     if(idx >= boxes.size() || idx < 0) return 1;
        //     boxes.erase(boxes.begin() + idx);
        //     for(int i = idx; i < boxes.size(); i++) {
        //         boxes[i].y1 = y + yLen * i;
        //         boxes[i].y2 = boxes[i].y1 + yLen;
        //     }
        //     return 0;
        // }

        // int updateText(int idx, string txt) {
        //     if(idx >= boxes.size() || idx < 0) return 1;
        //     boxes[idx].text = txt;
        //     return 0;
        // }

        // Draw all boxes to the screen
        void draw(sf::RenderWindow &window) {
            for(auto box: boxes) {
                box->draw(window);
            }
        }    
};