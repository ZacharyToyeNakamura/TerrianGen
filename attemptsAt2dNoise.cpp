// all the things I tried for generating 2d noise

srand(time(0));

// Generate random numbers for the sin functions
for(int i  = 0; i < COUNT; i++) {
    // a[i] = rand() % aBound+1;
    // if(!(rand() % 2)) a[i] = 1.0/a[i]; // randomly inverse a value
    a[i] = randDouble(0, 500);
    // a[i] = 1.0/a[i];
    // b[i] = rand() % bBound;
    // if(rand() % 2) b[i] *= -1; // randomly negative b value
    // b2[i] = rand() % bBound;
    // if(rand() % 2) b2[i] *= -1; // randomly negative b2 value
    // c[i] = rand() % cBound;
    b[i] = randDouble(0, 1000); // Only needs to be 0 to 2pi ?
    b2[i] = randDouble(0, 1000); // Only needs to be 0 to 2pi ?
    c[i] = randDouble(0, 1000);
    // c[i] = randDouble(0, 1);
    // k[i] = rand() % kBound+1;
    k[i] = randDouble(0, 1000);
    // if(rand() % 2) k[i] = 1.0/k[i]; // randomly inverse k value
    rotation[i] = randDouble(0, 90);
}
std::sort(k, k+COUNT);
// Debug: prints all waves
// for(int i = 0; i < COUNT; i++) cout << k[i]<< "sin("<<a[i]<<"(x -"<<b[i]<<")) + "<< k[i]<< "(sin("<<a[i]<<"(y -"<<b2[i]<<")) + "<<c[i]<<endl;



double minVal = INF, maxVal = -INF; 
// Testing a large amount of values (not all) to find the min and max to normalize vales to between 0 and 1
for(int x = 0; x < SIZEX; x++) {
    for(int y = 0; y < SIZEY; y++) {
        double tot = 0;
        for(int i = 0; i < COUNT; i++) {
            // Previous attempts at getting noise
            // tot += (k[i] * sin(a[i] * (x - midX - b[i])) + (k[i] * sin(a[i] * (y - midY - b2[i])) + c[i])) / (double)COUNT;
            // tot += (k[i] * sin(a[i] * (x*stepper - midX - b[i])) + (k[i] * sin(a[i] * (y*stepper - midY - b2[i])) + c[i])) / (double)(i + 1);
            // graph[x][y] += (k[i] * sin(a[i] * (x - midX - b[i])) + (k[i] * sin(a[i] * (y - midY - b[i])) + c[i]));
            tot += (k[i]*sin(a[i]*(x*stepper*cos(rotation[i])-y*stepper*sin(rotation[i])-b[i])) + k[i]*sin(a[i]*(x*stepper*sin(rotation[i])+y*stepper*cos(rotation[i])-b2[i]))) / (double)(i + 1);
        }
        if(tot < minVal) minVal = tot;
        if(tot > maxVal) maxVal = tot;
    }
}


// Normalize numbers from 0 to 1 from now on after getting the range using
// val = (((generated value - minVal)) / (maxVal - minVal))
for(int x = 0; x < SIZEX; x++) {
    for(int y = 0; y < SIZEY; y++) {
        double tot = 0;
        for(int i = 0; i < COUNT; i++) {
            // Previous attempts at getting noise
            // tot += (k[i] * sin(a[i] * (x - midX - b[i])) + (k[i] * sin(a[i] * (y - midY - b2[i])) + c[i])) / (double)COUNT;
            // tot += (k[i] * sin(a[i] * (x*stepper - midX - b[i])) + (k[i] * sin(a[i] * (y*stepper - midY - b2[i])) + c[i])) / (double)COUNT;
            // tot += (k[i] * sin(a[i]*(x*stepper - midX - b[i] + a[i]*(y*stepper - midY - b2[i]))) + c[i]) / (double)COUNT;
            // tot += (k[i] * sin(a[i]*(x*stepper - midX - b[i] + a[i]*(y*stepper - midY - b2[i]))) + c[i]) / (double)(i + 1);
            
            // graph[x][y] += (k[i] * sin(a[i] * (x - midX - b[i])) + (k[i] * sin(a[i] * (y - midY - b[i])) + c[i]));
            tot += (k[i]*sin(a[i]*(x*stepper*cos(rotation[i])-y*stepper*sin(rotation[i])-b[i])) + k[i]*sin(a[i]*(x*stepper*sin(rotation[i])+y*stepper*cos(rotation[i])-b2[i]))) / (double)(i + 1);
        }
        // tot = abs(tot);
        graph[x][y] = (tot - minVal) / (maxVal - minVal);
    }
}