template<typename T>HD ray<T>::ray():t(0),tmx(fsytd::lim<T>::max()){}
template<typename T>HD ray<T>::ray(const vec3<T,P>&o,const vec3<T,V>&d,T t,T tmx,T tmn,T time):o(o),d(d),t(t),tmx(tmx),tmn(tmn),time(time){};
template<typename T>HD T ray<T>::operator()(T t){return o+t*d;}
