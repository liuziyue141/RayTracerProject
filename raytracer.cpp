#include "variables.h"
//figure out importation 
#include <FreeImage.h>
#include "Transform.h"
#include "main.cpp"


vec3 rayThruPixel(int x, int y){
    
    //computing alpha and beta values for a corresponding pixel
    float alpha = glm::tan(fovx/2) * (y - (w/2))/(w/2);
    float beta= glm::tan(fovy/2) * ((h/2) - x)/(x/2);
    vec3 rayVector = vec3(alpha, beta, -1.0f);
    rayVector = glm::normalize(rayVector);
    return rayVector;
}

float checkIntersection(vec3 Ray){
    //iterating over the total number of objects
    
    //modify if necessary
    float currentMin = FLT_MAX

    for(int i = 0; i < numobjects; i++){
        if(objects[i].type == "sphere"){
            
            //unsure if oc, a, b, c are correct, need to verify.
            vec3 oc = eye - objects[i].sphere_loc;

            float a = glm::dot(Ray, Ray);
            float b = 2.0f * glm::dot(oc, Ray);
            float c = glm::dot(oc, oc) - objects[i].sphere_rad * objects[i].sphere_rad;
            float discriminant = (b * b) - (4 * a * c);
            if(discriminant < 0){
                //we do NOT consider this particular object then
                continue;
            }
            if(discriminant >= 0){
                float t1 = (-b + glm::sqrt(discriminant))/((2.0f)* a);
                float t2 = (-b - glm::sqrt(discriminant))/((2.0f)* a);
                if(t1 >= 0.0f && t2 >= 0.0f){
                    currentMin = min(t, min(t1, t2));
                }
                else if(t1 >= 0.0f){
                    currentMin = min(t, t1);
                }
                else if(t2 >= 0.0f){
                    currentMin = min(t, t2);
                }
            }

        else if(objects[i].type == "tri"){
            vec3 term1 = objects[i].tri_v3 - objects[i].tri_v1;
            vec3 term2 = objects[i].tri_v2 - objects[i].tri_v1;
            vec3 numerator = glm::cross(term1, term2);
            vec3 normal = glm::normalize(numerator);

        }
    }
    
    //in the end, final check to make sure that currentMin isn't the original value.
    if(currentMin == FLT_MAX){
        return -1.0f;
    }
    return currentMin;
}

void traceRay(){
    
    FreeImage_Initialise();

    //update 24 if needed
    //whether it is 24 or 32

    FITBITMAP* image = FreeImage_Allocate(w, h, 24);
    RGBQUAD color;

    //need to apply modelview transformation prior to for-loop
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            color.rgbRed = 0;
            color.rgbGreen = (double) i / w * 255.0;
            color.rgbBlue = (double) j / h * 255.0;
            vec3 currentRayVector = rayThruPixel(i, j);
            bool intersect = checkIntersection(currentRayVector);
            FreeImage_SetPixelColor(image, i, j, &color);
        }
    }

    if(freeImage_Save(FIF.PNG, image, "result.png", 0)){
        cout << "completed" << endl;
    }

    FreeImage_DeInitialise();
}
