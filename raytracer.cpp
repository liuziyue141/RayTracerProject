#include "raytracer.h"
using namespace std;
struct Ray {
    vec3 P0;
    vec3 P1;
};

struct id_norm_minT {
    int objectId;
    vec3 normal;
    float min_t;
};

id_norm_minT Intersect(Ray &ray){
    int objectId = -1;
    //vec3 P_return;
    vec3 normal = vec3(0,0,0);
    float min_t = FLT_MAX;
    for (int i = 0 ; i < numobjects ; i++) {
        object* obj = &(objects[i]);
        if(obj->type == "sphere"){
            vec3 P0 = vec3(obj->inverse_transform*vec4(ray.P0, 1.0f));
            vec3 P1 = vec3(obj->inverse_transform*vec4(ray.P1, 0.0f));
            float a = glm::dot(P1, P1);
            float b = 2.0f * glm::dot(P1, P0-(obj->sphere_loc));
            float c = glm::dot(P0 - obj->sphere_loc, P0 - obj->sphere_loc) - obj->sphere_rad*obj->sphere_rad;
            float discriminant = b*b-4*a*c;
            if(discriminant>=0){
                float root_minus = (-b-sqrt(discriminant))/(2*a);
                float root_plus = (-b+sqrt(discriminant))/(2*a);
                if(root_minus>0&&min_t>root_minus){
                    min_t = root_minus;
                    objectId = i;
                    normal = glm::normalize(vec3(glm::transpose(obj->inverse_transform) * vec4((min_t*P1+P0-obj->sphere_loc), 0)));

                }else if (root_plus>0&&min_t>root_plus){
                    min_t = root_plus;
                    objectId = i;
                    normal = glm::normalize(vec3(glm::transpose(obj->inverse_transform) * vec4((min_t*P1+P0-obj->sphere_loc), 0)));

                }
                //recover p --> normalize (p - center)
                // vec3 pos = vec3(obj->transform*vec4(P0+P1*min_t, 1));
                // normal = vec3(obj->inverse_transform*vec4(pos, 1))-obj->sphere_loc;
                // normal = vec3(glm::transpose(obj->inverse_transform)*(vec4(normal, 0)));
                // normal = glm::normalize(normal);
            }


            // ray.P0 = vec3(obj->transform*vec4(ray.P0, 1.0f));
            // ray.P1 = vec3(obj->transform*vec4(ray.P1, 0.0f));

        }else{
            vec3 A = obj->tri_v1;
            vec3 B = obj->tri_v2;
            vec3 C = obj->tri_v3;
            vec3 n = glm::normalize(glm::cross((C-A), (B-A)));
            //vec3 n = glm::normalize(glm::cross((B-A), (C-A)));
            float t = (glm::dot(A, n)-glm::dot(ray.P0, n))/glm::dot(ray.P1, n);
            if(t<min_t && t>0){
                vec3 P = ray.P0 + ray.P1*t ;
                float det = (C.x - A.x)*(B.y - A.y) - (B.x - A.x)*(C.y - A.y);
                float factor_beta = (C.x - A.x)*(P.y - A.y) - (P.x - A.x)*(C.y - A.y);
                float factor_gamma = (B.y - A.y)*(P.x - A.x) - (B.x - A.x)*(P.y - A.y);
                if(A.x == B.x && B.x == C.x){
                    det = (C.z - A.z)*(B.y - A.y) - (B.z - A.z)*(C.y - A.y);
                    factor_beta = (C.z - A.z)*(P.y - A.y) - (P.z - A.z)*(C.y - A.y);
                    factor_gamma = (B.y - A.y)*(P.z - A.z) - (B.z - A.z)*(P.y - A.y);
                }else if(A.y == B.y && B.y == C.y){
                    det = (C.x - A.x)*(B.z - A.z) - (B.x - A.x)*(C.z - A.z);
                    factor_beta = (C.x - A.x)*(P.z - A.z) - (P.x - A.x)*(C.z - A.z);
                    factor_gamma = (B.z - A.z)*(P.x - A.x) - (B.x - A.x)*(P.z - A.z);
                }
                float beta = factor_beta / det;
                float gamma = factor_gamma / det;
                float alpha = 1.0 - gamma - beta;
                if(alpha>=0 && beta>=0 && gamma >= 0){
                    min_t = t;
                    objectId = i;
                    if(obj->type == "trinormal"){
                        normal = alpha * obj->tri_norm_v1 + beta * obj->tri_norm_v2 + gamma * obj->tri_norm_v3;
                    }
                    else{
                        normal = -n;
                    }
                }      
            }
        }
    }
    id_norm_minT result;
    result.objectId = objectId;
    result.normal = normal;
    result.min_t = min_t;
    return result;
}



vec3 ComputeLight (vec3 direction, vec3 lightcolor, vec3 normal, vec3 halfvec, vec3 mydiffuse,
     vec3 myspecular, float myshininess) {
        vec3 retval;

        float nDotL = dot(normal, direction)  ;         
        vec3 lambert = mydiffuse * lightcolor * max(nDotL, 0.0f) ;  

        float nDotH = dot(normal, halfvec) ; 
        vec3 phong = myspecular * lightcolor * pow (max(nDotH, 0.0f), myshininess) ;
        retval = (lambert + phong) ; 
        return retval ;            
} 


vec3 findColor(Ray ray, int objectId, float min_t, vec3 normal){
    vec3 intersection = ray.P0 + ray.P1 * min_t;
    vec3 finalcolor = vec3(objects[objectId].ambient[0], objects[objectId].ambient[1], objects[objectId].ambient[2])
                    + vec3(objects[objectId].emission[0], objects[objectId].emission[1], objects[objectId].emission[2]);

    for(int i = 0; i<numLights; i++){
            vec4 lightpos = vec4(lightposn[4*i], lightposn[4*i+1], lightposn[4*i+2], lightposn[4*i+3]);
            vec3 light_color = vec3(lightcolor[3*i], lightcolor[3*i+1], lightcolor[3*i+2]);
            vec3 eyedirn = -normalize(ray.P1);
            vec3 diffuse = vec3(objects[objectId].diffuse[0], objects[objectId].diffuse[1], objects[objectId].diffuse[2]);
            vec3 specular = vec3(objects[objectId].specular[0], objects[objectId].specular[1], objects[objectId].specular[2]);
            float visibility = 1;
            if(lightpos.w==0){
                vec3 light_dirn = glm::normalize(vec3(lightpos.x, lightpos.y, lightpos.z) - intersection);
                vec3 halfvec = normalize(light_dirn + eyedirn);
                float distance = glm::length(vec3(lightpos.x, lightpos.y, lightpos.z) - intersection);
                float attentuation = 1/(constant_atten+linear_atten*distance+quadratic_atten*distance*distance);
                Ray light_to_object;
                light_to_object.P0 = vec3(lightpos);
                light_to_object.P1 = -light_dirn;
                id_norm_minT light2object = Intersect(light_to_object);
                if (light2object.min_t<glm::length(vec3(lightpos.x, lightpos.y, lightpos.z) - intersection)-1e-4){
                    // cout << "min t for camera ray " <<glm::length(vec3(lightpos.x, lightpos.y, lightpos.z) - intersection) << ", min t for light ray " << light2object.min_t <<endl;
                    // cout << " camera object " <<objectId << ", light ray object " << light2object.objectId <<endl;

                    visibility = 0;
                }
                finalcolor += attentuation*visibility*ComputeLight(light_dirn, light_color, normal, halfvec, diffuse, specular, shininess);
            }else{
                vec3 light_dirn = glm::normalize(vec3(lightpos.x, lightpos.y, lightpos.z));
                vec3 halfvec = normalize(light_dirn + eyedirn);
                float distance = glm::length(vec3(lightpos.x, lightpos.y, lightpos.z) - intersection);
                float attentuation = 1/(constant_atten+linear_atten*distance+quadratic_atten*distance*distance);
                // vec3 light_dirn = normalize(vec3(lightpos.x, lightpos.y, lightpos.z));
                // vec3 halfvec = normalize(light_dirn + eyedirn);
                // float min_T_light_to_intersection = (vec3(lightpos.x, lightpos.y, lightpos.z) - intersection).x / light_dirn.x;

                // finalcolor += ComputeLight(light_dirn, light_color, normal, halfvec, diffuse, specular, shininess);

            }
        }
    //cout << "color: r: " << finalcolor.x << ", g: " << finalcolor.y << ", b: " << finalcolor.z << endl;
    return finalcolor;
}

//using namespace glm;

Ray RayThruPixel(int i, int j){
    float alpha = tan(fovx/2.0f)*((i-(w/2.0f))/(w/2.0f));
    float beta = tan(fovy/2.0f)*((h/2.0f)-j)/(h/2.0f);
    vec3 w = glm::normalize(eye - center);
    vec3 u = glm::normalize(glm::cross(up, w));
    vec3 v = glm::cross(w, u);
    Ray ray;
    ray.P0 = eye;
    ray.P1 = glm::normalize(alpha*u+beta*v-w);
    return ray;
}
void traceRay(){
    FIBITMAP * bitmap = FreeImage_Allocate(w, h, 24);
    RGBQUAD color;
    Ray ray = RayThruPixel(0, w/2.0f);
    //if this is too slow, we change h and w order
    for(int i = 0; i<w; i++){
        for(int j = 0; j<h; j++){
            Ray ray = RayThruPixel(i, j);
            //int objectId = Intersect(ray);
            id_norm_minT result = Intersect(ray);
            int objectId = result.objectId;

            if(objectId>=0){
                object* obj = &(objects[objectId]);
                vec3 colorVector = findColor(ray, objectId, result.min_t, result.normal);
                // cout << "min t" << result.min_t << endl;
                // cout << "normal for pixel: x: " << result.normal.x << ", y: " << result.normal.y << ", z: " << result.normal.z << endl;

                // cout << "color at pixel: width: " << i << ", height: " << j << ", colorVector: " << colorVector.x << ", " << colorVector.y << ", " << colorVector.z << endl;
                color.rgbRed = colorVector.z* 255.0f;
                color.rgbGreen = colorVector.y * 255.0f;
                color.rgbBlue = colorVector.x * 255.0f;

            }else{
                color.rgbRed = 0;
                color.rgbGreen = 0;
                color.rgbBlue = 0;
            }
            FreeImage_SetPixelColor(bitmap, i, h-j-1, &color);
        }
    }
    if (FreeImage_Save(FIF_PNG, bitmap, "test.png", 0)){
        cout << "Image successfully saved!" << endl;
    }
}