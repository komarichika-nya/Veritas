template<typename T>
HD surface<T>::surface(){}
template<typename T>
HD surface<T>::surface(const vec3<T,P>&pos,const vec3<T,V>&face_normal,const vec3<T,V>&shading_normal,T u,T v,const vec3<T,V>&dpu,const vec3<T,V>&dpv,const vec3<T,V>&wo):pos(pos),face_normal(face_normal),shading_normal(shading_normal),u(u),v(v),dpu(dpu),dpv(dpv),wo(wo){}
template<typename T>
HD surface<T>::surface(const vec3<T,P>&pos,const vec3<T,V>&face_normal):pos(pos),face_normal(face_normal){}
template<typename T>
HD surface<T>::surface(const vec3<T,P>&pos,const vec3<T,V>&face_normal,const vec3<T,V>&wo):pos(pos),face_normal(face_normal){}
