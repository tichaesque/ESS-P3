class Goldfish {
  
  // position of goldfish
  float posX;
  float posY; 
  
  Goldfish(float posX_, float posY_) {
    posX = posX_;
    posY = posY_;
  }
  
  void update(float posX_, float posY_) {
    posX = posX_;
    posY = posY_; 
  }
  
}