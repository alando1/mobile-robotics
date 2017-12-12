#include <StackArray.h>

struct Cell
{
    char walls[4] = { '-', '-', '-', '-' };
    bool hasNotBeenVisited = true;
};

char* directions[] = {"WEST", "NORTH", "EAST", "SOUTH"};

enum CellState 
{
    CHECK_CELL,                     //check distance sensors, update walls of cell
    CHOOSE_DIRECTION,               //using wall data, reorient position to move to next unvisited neighbor
    TURN_LEFT,                      //14 ticks left
    TURN_RIGHT,                     //14 ticks right
    TURN_AROUND,                    //28 ticks
    MOVE_FORWARD,                   //transitions to next cell and stops in the center.
    MAPPING_COMPLETE
};

char row1[16], row2[16], row3[16], row4[16];
bool printState1 = false, printState2 = false, printState3 = false;
bool mapping = true;
CellState currentMapState = CHECK_CELL;
int currentCell = 0;
int visitedCells = 0;
//float threshold = 7.5f;     //maze at home, cell size 15x15.    
float threshold = 12.0f;     //c4lab maze 18x18
Cell grid[16];
StackArray<int> myStack;
CellState dirTable[4][4];   // Array storing motions needed to go from one orientation to another

//function prototypes
void updateMappingStates();
void endMap();
void checkCell();
void checkCell2();
void moveForward();
void chooseDirection();
void initializeMaze();
void initializedirTable();
void rotate(int a, int b, int c);
bool neighborExists(int dir, int cellNum);
bool neighborIsOpen(int dir, int cellNum);
bool allNeighborsVisited(int cellNum);
int findUnvisitedCell(int cellNum);
int findUnvisitedCellRecurse(int cellNum);
CellState changeDir(int dir_i, int dir_f);
void doShortestPath();

void updateMappingStates()
{
    switch(currentMapState)
    {
        case CHECK_CELL:                checkCell();                break;
        case CHOOSE_DIRECTION:          chooseDirection();          break;
        case MOVE_FORWARD:              moveForward();              break;
        case TURN_LEFT:                 rotate(-22, 20, 14);      break;
        case TURN_RIGHT:                rotate(25, -24, 14);       break;
        case TURN_AROUND:               rotate(25, -25, 27);      break;
        case MAPPING_COMPLETE:          endMap();                   break;
        default:    break;
    }
}

void checkCell()
{
    float sensorData[3];
    delay(500); // ?

    bool testFunc;

    //do sensor readings if cell has not been visited.
    if (grid[currentCell].hasNotBeenVisited)
    {
        //check/update wall data
        sensorData[0] = getCombinedDistance(SLEFT);
        sensorData[1] = getCombinedDistance(SFRONT);
        sensorData[2] = getCombinedDistance(SRIGHT);
        
        // loop through the 3 sensors (L, F, R)
        for (int i = 0; i < 3; ++i)
        {
            // compute a wall index from current orientation plus an offset
            // from [-1, 1], wrapping around edges
            int wi = orientation + i - 1;
            if (wi < 0)
                wi = 4 + wi;
            else if (wi > 3)
                wi = wi - 4;

            // if sensor reads below threshold, mark as wall
            if (sensorData[i] < threshold)
                grid[currentCell].walls[wi] = 'w';
            else
                grid[currentCell].walls[wi] = 'o';
        }



//        lcd.clear();
//        lcd.setCursor(7, 1);
//        int temp = (int)sensorData[0];
//        lcd.print(temp);
//        lcd.setCursor(10, 1);
//        temp = (int)sensorData[1];
//        lcd.print(temp);
//        lcd.setCursor(13, 1);
//        temp = (int)sensorData[2];
//        lcd.print(temp);

        grid[currentCell].hasNotBeenVisited = false;
        visitedCells++;
        myStack.push(currentCell);
        
//        Serial.print("# Cells Visited:  ");
//        Serial.println(visitedCells);
//
//        
//        Serial.print(" Current Cell:  ");
//        Serial.println(currentCell);
//
//        testFunc = allNeighborsVisited(currentCell);
//        Serial.print("ANV:  ");
//        Serial.println(testFunc);
        
//        lcd.setCursor(14, 0);
//        lcd.print(visitedCells);
        delay(1000);
    }
    lcd.clear();
    lcd.setCursor(0,0);
    char str[16];
    for(int i = 0; i < 16; i++)
    {
        if(grid[i].hasNotBeenVisited)
            str[i] = '0';
        else
            str[i] = 'X';
    }
    lcd.print(str);
    
    
//    lcd.setCursor(8, 0);
//    lcd.print(testFunc);
//    lcd.print('T');
    
    currentMapState = CHOOSE_DIRECTION;
    //lcd.setCursor(0, 0);
    //lcd.print(grid[currentCell].walls);
    lcd.setCursor(0, 1);
    lcd.print('G');
    lcd.print(currentCell);
    lcd.setCursor(3, 1);
    lcd.print('O');

    switch (orientation)
    {
        case WEST:      lcd.print('W');         break;
        case NORTH:     lcd.print('N');         break;
        case EAST:      lcd.print('E');         break;
        case SOUTH:     lcd.print('S');         break;
        default:        lcd.print('?');         break;
    }
}

void chooseDirection()
{
    //find all neighbors that are open cells   
    if(neighborIsOpen(WEST, currentCell))
    {
        if(grid[currentCell - 1].hasNotBeenVisited)
        {
            currentMapState = changeDir(orientation, WEST);
            return;
        }
    }
    if(neighborIsOpen(NORTH, currentCell))
    {
        if(grid[currentCell - 4].hasNotBeenVisited)       //neighbor to north is open
        {
            currentMapState = changeDir(orientation, NORTH);
            return;
        }
    }
    
    if(neighborIsOpen(EAST, currentCell))       //neighbor to east is open
    {
        if(grid[currentCell + 1].hasNotBeenVisited)
        {
            currentMapState = changeDir(orientation, EAST);
            return;
        }
    }
    if(neighborIsOpen(SOUTH, currentCell))       //neighbor to south is open
    {
        if(grid[currentCell + 4].hasNotBeenVisited)
        {
          currentMapState = changeDir(orientation, SOUTH);
          return;
        }
    }
    if(visitedCells != 16)                              //check to make sure there are unvisited cells
    {
       //int dir = findUnvisitedCell(currentCell);      //return direction needed to go to unvisited cell
       if(!myStack.isEmpty())
       {
            myStack.pop();
            
            int i = myStack.peek();
            int x = currentCell - i;

            switch(x)
            {
                case 1:     currentMapState = changeDir(orientation, WEST);   break;
                case -1:    currentMapState = changeDir(orientation, EAST);   break;
                case 4:     currentMapState = changeDir(orientation, NORTH);  break;
                case -4:    currentMapState = changeDir(orientation, SOUTH);  break;
            }
            
            //lcd.setCursor(5, 0);
            //lcd.print(dir);
       }
         //currentMapState = changeDir(orientation, dir);
    }
    else
    {
        currentMapState = MAPPING_COMPLETE;
    }

   //lcd.setCursor(8, 0);
   //lcd.print(currentState);

    return;
}

void moveForward()
{
    unsigned long counts[2];
    setSpeeds(100,100);
    resetCounts();
    getCounts(counts);
    while(counts[0] < 70)   //test maze 47, c4lab maze 70
    {
        getCounts(counts);
        testColorRecognition();
    }
    setSpeeds(0,0);
    delay(10);

    switch(orientation)
    {
        case WEST:      currentCell -= 1;   grid[currentCell].walls[2] = 'o'; break;
        case NORTH:     currentCell -= 4;   grid[currentCell].walls[3] = 'o'; break;
        case EAST:      currentCell += 1;   grid[currentCell].walls[0] = 'o'; break;
        case SOUTH:     currentCell += 4;   grid[currentCell].walls[1] = 'o'; break;
        default:        break;
    }

    currentMapState = CHECK_CELL;

}

void rotate(int a, int b, int ticks)
{
    unsigned long counts[2];
    resetCounts();
    setSpeeds(a, b);
    getCounts(counts);
    while(counts[0] < ticks)
        getCounts(counts);

    setSpeeds(0,0);
    delay(500);

    currentMapState = MOVE_FORWARD;
}

bool neighborExists(int dir, int cellNum)
{
    switch(dir)
    {
        case WEST:  
        {
            if((((cellNum - 1) % 4) != 3) && (cellNum != 0))
                return true;
            else
                return false;
        } 
        case NORTH: 
        {
            if((cellNum - 4) > -1) 
                return true;
            else
                return false;
        }
        case EAST:  
        {
            if((((cellNum + 1) % 4) != 0) && (cellNum != 15) )
                return true;
            else
                return false;
        }
        case SOUTH:
        {
            if((cellNum + 4) < 16)
                return true;
            else
                return false;
        }
        default:    break;
    }
    return false;
}

bool neighborIsOpen(int dir, int cellNum)
{
    if (neighborExists(dir, cellNum))
    {
        
        if (grid[cellNum].walls[dir] == 'o')
            return true;
        else
            return false;
         
    }
    else
        return false;
}

bool allNeighborsVisited(int cellNum)
{
    Serial.print("current cell: ");
    Serial.print(cellNum);
            
    int d[4] = {-1, -4, 1, 4};

    // loop through the directions
    for(int i = 0; i < 4; i++)
    {
        // if a neighbor is open in that direction
//        if (neighborIsOpen(i, cellNum))
//        {
//            // check if it's been visited
//            if(grid[cellNum + d[i]].hasNotBeenVisited)
//                return false;
//        }
        
        if(neighborExists(i, cellNum))
        {
            //Serial.print(", Initial Orientation: ");
            Serial.print(", checking: ");
            Serial.print(directions[i]);
            //Serial.print(", d[i]: ");
            //Serial.println(d[i]);
            Serial.print(" (");
            Serial.print(cellNum + d[i]);
            Serial.print(") ");
                        
            if(grid[cellNum + d[i]].hasNotBeenVisited)
            {
                Serial.println();
                return false;
            }
        }
    }
    Serial.println();
    
    return true;
}

int findUnvisitedCell(int cellNum)
{
    // W, N, E, S
    int neighbors[4] = {100, 100, 100, 100};
    int result = -1;
    int minD = 100;

    // loop through the 4 directions
    for (int i = 0; i < 4; ++i)
    {
        // if there's a neighbor in that direction, recurse there
        if (neighborIsOpen(i, cellNum)) // YOU CHANGED THIS
        {
            switch (i)
            {
                // store the accumulated distance to the nearest unvisited cell from here
                case WEST:  neighbors[WEST] = findUnvisitedCellRecurse(cellNum - 1);    break;
                case NORTH: neighbors[NORTH] = findUnvisitedCellRecurse(cellNum - 4);   break;
                case EAST:  neighbors[EAST] = findUnvisitedCellRecurse(cellNum + 1);    break;
                case SOUTH: neighbors[SOUTH] = findUnvisitedCellRecurse(cellNum + 4);   break;
            }
        }
        
        // keep a running minimum direction to go
        if (neighbors[i] < minD)
        {
            minD = neighbors[i];
            result = i;
        }
    }

    // return closest found direction to go
    return result;
}

int findUnvisitedCellRecurse(int cellNum)
{
    int neighbors[4] = {100, 100, 100, 100};

    if(allNeighborsVisited(cellNum))
    {
        // a short array to hold the offsets for the 4 directions
        int d[4] = {-1, -4, 1, 4};

        // loop through the 4 directions
        for (int i = 0; i < 4; ++i)
        {
            // if the neighbor is open, recurse there
            if (neighborIsOpen(i, cellNum))
                neighbors[i] = findUnvisitedCellRecurse(cellNum + d[i]) + 1;
        }
        
        // sort the 4 distances computed
        intbubbleSort(neighbors, 4);

        // return the smallest one
        return neighbors[0];
    }
    else
        return 1;
}

CellState changeDir(int dir_i, int dir_f)
{
    orientation = dir_f;
    return dirTable[dir_i][dir_f];
}


void initializeMaze()
{   
    currentCell = startLocation;
    //initialize outer border walls
    grid[0].walls[0] = 'w';
    grid[4].walls[0] = 'w';
    grid[8].walls[0] = 'w';
    grid[12].walls[0] = 'w';

    grid[0].walls[1] = 'w';
    grid[1].walls[1] = 'w';
    grid[2].walls[1] = 'w';
    grid[3].walls[1] = 'w';

    grid[3].walls[2] = 'w';
    grid[7].walls[2] = 'w';
    grid[11].walls[2] = 'w';
    grid[15].walls[2] = 'w';

    grid[12].walls[3] = 'w';
    grid[13].walls[3] = 'w';
    grid[14].walls[3] = 'w';
    grid[15].walls[3] = 'w';    

}

void initializedirTable()
{
    dirTable[0][0] = MOVE_FORWARD; 
    dirTable[0][1] = TURN_RIGHT;
    dirTable[0][2] = TURN_AROUND;
    dirTable[0][3] = TURN_LEFT;
    dirTable[1][0] = TURN_LEFT;
    dirTable[1][1] = MOVE_FORWARD;
    dirTable[1][2] = TURN_RIGHT;
    dirTable[1][3] = TURN_AROUND;
    dirTable[2][0] = TURN_AROUND;
    dirTable[2][1] = TURN_LEFT;
    dirTable[2][2] = MOVE_FORWARD;
    dirTable[2][3] = TURN_RIGHT;
    dirTable[3][0] = TURN_RIGHT;
    dirTable[3][1] = TURN_AROUND;
    dirTable[3][2] = TURN_LEFT;
    dirTable[3][3] = MOVE_FORWARD;
    
}

void endMap()
{
    mapping = false;
    lcd.clear();
    lcd.setCursor(0, 0);

    lcd.print("     DONE");
    delay(50);    

    row1[0] = grid[0].walls[0];
    row1[1] = grid[0].walls[1];
    row1[2] = grid[0].walls[2];
    row1[3] = grid[0].walls[3];
    row1[4] = grid[1].walls[0];
    row1[5] = grid[1].walls[1];
    row1[6] = grid[1].walls[2];
    row1[7] = grid[1].walls[3];
    row1[8] = grid[2].walls[0];
    row1[9] = grid[2].walls[1];
    row1[10] = grid[2].walls[2];
    row1[11] = grid[2].walls[3];
    row1[12] = grid[3].walls[0];
    row1[13] = grid[3].walls[1];
    row1[14] = grid[3].walls[2];
    row1[15] = grid[3].walls[3];
    
    row2[0] = grid[4].walls[0];
    row2[1] = grid[4].walls[1];
    row2[2] = grid[4].walls[2];
    row2[3] = grid[4].walls[3];
    row2[4] = grid[5].walls[0];
    row2[5] = grid[5].walls[1];
    row2[6] = grid[5].walls[2];
    row2[7] = grid[5].walls[3];
    row2[8] = grid[6].walls[0];
    row2[9] = grid[6].walls[1];
    row2[10] = grid[6].walls[2];
    row2[11] = grid[6].walls[3];
    row2[12] = grid[7].walls[0];
    row2[13] = grid[7].walls[1];
    row2[14] = grid[7].walls[2];
    row2[15] = grid[7].walls[3];

    row3[0] = grid[8].walls[0];
    row3[1] = grid[8].walls[1];
    row3[2] = grid[8].walls[2];
    row3[3] = grid[8].walls[3];
    row3[4] = grid[9].walls[0];
    row3[5] = grid[9].walls[1];
    row3[6] = grid[9].walls[2];
    row3[7] = grid[9].walls[3];
    row3[8] = grid[10].walls[0];
    row3[9] = grid[10].walls[1];
    row3[10] = grid[10].walls[2];
    row3[11] = grid[10].walls[3];
    row3[12] = grid[11].walls[0];
    row3[13] = grid[11].walls[1];
    row3[14] = grid[11].walls[2];
    row3[15] = grid[11].walls[3];

    row4[0] = grid[12].walls[0];
    row4[1] = grid[12].walls[1];
    row4[2] = grid[12].walls[2];
    row4[3] = grid[12].walls[3];
    row4[4] = grid[13].walls[0];
    row4[5] = grid[13].walls[1];
    row4[6] = grid[13].walls[2];
    row4[7] = grid[13].walls[3];
    row4[8] = grid[14].walls[0];
    row4[9] = grid[14].walls[1];
    row4[10] = grid[14].walls[2];
    row4[11] = grid[14].walls[3];
    row4[12] = grid[15].walls[0];
    row4[13] = grid[15].walls[1];
    row4[14] = grid[15].walls[2];
    row4[15] = grid[15].walls[3];
    
    
}

void doShortestPath()
{
    
}

