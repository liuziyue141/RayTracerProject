#include "raytracer.h"
using namespace std;

//using namespace glm;
int Intersect(vec3 ray){
    int objectId = -1;
    float min_t = FLT_MAX;
    for (int i = 0 ; i < numobjects ; i++) {
        object* obj = &(objects[i]);
        if(obj->type == "sphere"){
            float a = glm::dot(ray, ray);
            float b = 2 * glm::dot(ray, -(obj->sphere_loc));
            float c = glm::dot(obj->sphere_loc, obj->sphere_loc) - obj->sphere_rad*obj->sphere_rad;
            float discriminant = b*b-4*a*c;
            if(discriminant>=0){
                float root_minus = (-b-sqrt(discriminant))/(2*a);
                float root_plus = (-b+sqrt(discriminant))/(2*a);
                if(root_minus>0&&min_t>root_minus){
                    min_t = root_minus;
                    objectId = i;
                }else if (root_plus>0&&min_t>root_plus){
                    min_t = root_plus;
                    objectId = i;
                }
            }
        }else{
            vec3 A = obj->tri_v1;
            vec3 B = obj->tri_v2;
            vec3 C = obj->tri_v3;
            vec3 normal = glm::normalize(glm::cross((C-A), (B-A)));
            float t = glm::dot(A, normal)/glm::dot(ray, normal);
            if(t<min_t){
                vec3 P = ray*t;
                float d00 = glm::dot(B-A, B-A);
                float d01 = glm::dot(B-A, C-A);
                float d11 = glm::dot(C-A, C-A);
                float d20 = glm::dot(P-A, B-A);
                float d21 = glm::dot(P-A, C-A);
                float denom = d00*d11-d01*d01;
                float v = (d11 * d20 - d01 * d21) / denom;
                float w = (d00 * d21 - d01 * d20) / denom;  
                if(v>0 && w>0 && 1.0f - v- w > 0){
                    min_t = t;
                    objectId = i;
                }      
            }
        }
    }
    return objectId; 
}
vec3 RayThruPixel(int i, int j){
    float alpha = tan(fovx/2.0f)*((j-(w/2.0f))/(w/2.0f));
    float beta = tan(fovy/2.0f)*((h/2.0f)-i)/(h/2.0f);
    return vec3 (glm::normalize(vec3(alpha, beta, -1.0f)));
}

void traceRay(){
    FIBITMAP * bitmap = FreeImage_Allocate(w, h, 24);
    modelview = glm::lookAt(eye, center, up);
    RGBQUAD color;
    for (int i = 0 ; i < numobjects ; i++) {
        object* obj = &(objects[i]);
        obj->transform = modelview*obj->transform;//we need to modify this for scale
    }
    for(int i = 0; i<h; i++){
        for(int j = 0; j<w; j++){
            vec3 ray = RayThruPixel(i, j);
            int objectId = Intersect(ray);
            object* obj = &(objects[i]);
            color.rgbRed = (double) obj->diffuse[0]*255.0;
            color.rgbGreen = (double) obj->diffuse[1]*255.0;
            color.rgbBlue = (double) obj->diffuse[2]*255.0;
            FreeImage_SetPixelColor(bitmap, i, j, &color);
        }
    }
    if (FreeImage_Save(FIF_PNG, bitmap, "test.png", 0)){
        cout << "Image successfully saved!" << endl;
    }
}



