#include<cmath> 
#include"conf/conf.hpp"
#include"camera/camera.hpp"
template<typename tp,typename T,typename LensT,typename Filter>
HD Camera<tp,T,LensT,Filter>::Camera(Pose<T>&pos,LensT&lens,shut<T>&sh,Film<Filter,T>&film,int w,int h):
    pos(pos),lens(lens),sh(sh),film(&film),w(w),h(h){}
template<typename tp,typename T,typename LensT,typename Filter>
HD void Camera<tp,T,LensT,Filter>::create(const ray<T>*r,cameraSample<T>&sampler)const{
    static_cast<tp*>(this)->generateRay(r,sampler);
};
template<typename T,typename LensT,typename Filter>
HD Projective<T,LensT,Filter>::Projective(Pose<T>&pos,LensT&lens,shut<T>&sh,Film<Filter,T>&film,int w,int h,T fov):
    base(pos,lens,sh,film,w,h),fov(fov),hf_h(tan(fov/2*3.1415926535/180.0)),hf_w(T(w)/T(h)*hf_h){};
template<typename T,typename LensT,typename Filter>
HD void Projective<T,LensT,Filter>::generateRay(ray<T>*r,cameraSample<T>&sam)const{
    T fx=sam.px,fy=sam.py;
    T ndcx=(T(2)*fx)/this->w-T(1),ndcy=T(1)-(T(2)*fy)/this->h;
    T camx=ndcx*this->hf_w,camy=ndcy*this->hf_h;
    auto&pos=this->pos;auto &lens=this->lens;auto sh=this->sh;
    vec3<T,V>dir=nor(pos.f+pos.r*camx+pos.u*camy);
    lens.build(sam,r,pos,dir);
    r->t=sh.sample(sam.t);
}
