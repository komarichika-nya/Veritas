import mitsuba as mi
import drjit as mi
mi.set_variant('llm_ad_rgb')
sence=mi.load_dict(mi.cornell_box())
cam_o=mi.Point3f(0,1,3)
cam_d=mi.normalize(mi.Vector3f(0,-0.5,-1));
cam_w,cam_h=2.0,2.0
wh=(800,600)

img=mi.render(sence,spp=256)
mi.Bitmap(img).write('a.exr')

