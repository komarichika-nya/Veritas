template<typename tp,typename T>HD vec2<T,P>Filter<tp,T>::radius(){return static_cast<const tp*>(this)->radius();}
template<typename tp,typename T>
HD T Filter<tp,T>::evaluate(const vec2<T,P>&p){return static_cast<const tp*>(this)->evaluate(p);}
template<typename tp,typename T>
HD T Filter<tp,T>::integral(){return static_cast<const tp*>(this)->integral();}
template<typename tp,typename T>
HD filterSample<T>Filter<tp,T>::sample(const vec2<T,P>&v)const{return static_cast<const tp*>(this)->sample(v);}

template<typename T>
HD Box<T>::Box()=default;
template<typename T>
HD Box<T>::Box(const vec2<T,P>&r):r(r){}
template<typename T>
HD vec2<T,P>Box<T>::radius()const{return r;}
template<typename T>
HD T Box<T>::integral()const{return T(4)*r.x*r.y;}
template<typename T>
HD T Box<T>::evaluate(const vec2<T,P>&p)const{return (fsytd::abs(p.x)<=r.x&&fsytd::abs(p.y)<=r.y)?T(1):T(0);}
template<typename T>
HD filterSample<T>Box<T>::sample(const vec2<T,P>&v)const{return{{lerp(-r.x,r.x,v.x),lerp(-r.y,r.y,v.y)},T(1)};}
