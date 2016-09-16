#include "powerUp.hpp"

powerUp::powerUp(int type,nTPoint pos,nTPoint size,bool haveGravity) {
    this->size=size;
    this->pos=pos;
    this->haveGravity=haveGravity;
    this->vSpeed=0;
    this->hSpeed=moveSpeed;
    this->type=type;
    this->self.push_back(this);
    //TODO:set texture
};

powerUp::powerUp(const powerUp& orig) {
}

powerUp::~powerUp() {
    for(int i=0;i<self.size();i++){
        if(self[i]==this){
            self.erase(self.begin()+i);
            break;
        }
    }
}

vector<void*> powerUp::self;
float powerUp::moveSpeed=20;

void powerUp::behave(){
    if(GL::isPaused)
        return;
    powerUp *pu; 
    for(int i=0;i<self.size();i++){
        pu=(powerUp*)self[i];
        if(pu->haveGravity){
            if(!Mechanics::checkNormalForce(pu->pos,pu->size)){
                pu->vSpeed+=Mechanics::getGravity()/GL::getFPS();
            }else if((pu->vSpeed<0&&Mechanics::getGravity()<0)||(pu->vSpeed>0&&Mechanics::getGravity()>0)){
                pu->vSpeed=0;
            }
        }
        if(pu->hSpeed!=0){
            pu->move(Util::left,pu->hSpeed/GL::getFPS());
        }
        if(pu->vSpeed>0){
            pu->move(Util::down,pu->vSpeed/GL::getFPS());
        }else if(pu->vSpeed<0){
            pu->move(Util::up,pu->vSpeed/GL::getFPS());
        }
       pu->checkCollisionWithPlayer(player->pos,player->size);
       pu->draw();
    }
}

void powerUp::move(int dir,float steeps){
    vector <mapCollision> var;
    bool condition=true;
    if(dir==Util::left||dir==Util::right){
      nTPoint tmp;
      tmp.setPoint(pos.x+steeps,pos.y,pos.z);
      var=Map::checkCollision(tmp,size);
      for(int i=0; i<var.size(); i++){
        if(var[i].collision.firstObj>=3)
          condition=false;
      }
      if(condition)
        pos.x+=steeps;
      else
        hSpeed=-moveSpeed;

    }else if(dir==Util::up){
      nTPoint tmp;
      tmp.setPoint(pos.x,pos.y-steeps,pos.z);
      var=Map::checkCollision(tmp,size);
      for(int i=0; i<var.size(); i++){
        if(var[i].collision.firstObj==2)
          condition=false;
      }
      if(condition)
        pos.y-=steeps;
    }else if(dir==Util::down){
      nTPoint tmp;
      tmp.setPoint(pos.x,pos.y+steeps,pos.z);
      var=Map::checkCollision(tmp,size);
      for(int i=0; i<var.size(); i++){
        if(var[i].collision.firstObj==1)
          condition=false;
      }
      if(condition){
        pos.y+=steeps;
      }
    }
}

void powerUp::checkCollisionWithPlayer(nTPoint pos,nTPoint size){
    //TODO:aplicar powerup (passa a posicao do player pra funcao)) aplica power up e deleta obj
}

void powerUp::draw(){
    if(GL::isPaused)
        return;
    glNewList(GLlist,GL_COMPILE_AND_EXECUTE);
    GL::drawTexture(Util::getCollisionRectangle(pos,size),GL::getColorByName("white"),tex,1);
    glEndList();
}