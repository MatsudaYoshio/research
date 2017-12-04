dlib::point get_center_point(const dlib::rectangle& r);

point get_center_point(const rectangle& r){
  return point((r.left()+r.right())/2, (r.top()+r.bottom()/2));
  
}


