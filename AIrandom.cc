#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Random


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
    
    bool posicion_valida_dwarf(const Pos& newpos,const vector<vector<bool>>& visited){
        return pos_ok(newpos) and not visited[newpos.i][newpos.j] and (cell(newpos).type==Cave or cell(newpos).type==Rock);
    }
    
    bool posicion_valida_wizard(const Pos& newpos,const vector<vector<bool>>& visited){
        return pos_ok(newpos) and not visited[newpos.i][newpos.j] and (cell(newpos).type==Cave or cell(newpos).type==Outside);
    }
    
    void balrog_troll(vector<vector<bool>>& visited){
        Pos bal_pos=unit(balrog_id()).pos;
        for (int p=-2; p<=2;++p){
            for (int k=-2; k<=2; ++k){
                if (pos_ok((bal_pos.i)+p,(bal_pos.j)+k)) visited[(bal_pos.i)+p][(bal_pos.j)+k]=true;
            }
        }
        vector<int> trolls_id=trolls();
        int n=trolls_id.size();
        for (int k=0; k<n; ++k){
            for (int p=-1; p<=1;++p){
                for (int u=-1; u<=1; ++u){
                    Pos troll_pos=unit(trolls_id[k]).pos;
                    if (pos_ok((troll_pos.i)+p,(troll_pos.j)+u)) visited[(troll_pos.i)+p][(troll_pos.j)+u]=true;
                }
            }
        }
    }
  
    void move_dwarves(){
        vector<int> D = dwarves(me());
        int n=D.size();
        for (int i=0; i<n; ++i){
            int id=D[i];
            Pos inicial=unit(id).pos;
            vector<vector<bool>> visited(60,vector<bool>(60,false));
            visited[inicial.i][inicial.j]=true;
            balrog_troll(visited);
            queue<pair<Pos,Dir>> qu;
            Dir newdir=None;
            qu.push(make_pair(inicial,newdir));
            bool primera=true;
            if (cell(inicial).type==Outside){
                bool found=false;
                while (not qu.empty() and not found){
                    Pos act_pos=qu.front().first;
                    Dir act_dir=qu.front().second; qu.pop();
                    Cell c=cell(act_pos);
                    newdir=act_dir;
                    if (c.type==Cave){
                        found=true;
                        command(id, act_dir);
                    }
                    else { //mirar veins
                        int rand=random(0,8);
                        for (int p=0; p<8 and not found; ++p){
                            Pos newpos;
                            ++rand;
                            newpos=act_pos+Dir(rand%8);
                            if (pos_ok(newpos) and not visited[newpos.i][newpos.j] and (cell(newpos).type==Cave or cell(newpos).type==Outside)){
                                if (primera) {
                                    if (rand%8==0) newdir=Bottom;
                                    else if (rand%8==1) newdir=BR;
                                    else if (rand%8==2) newdir=Right;
                                    else if (rand%8==3) newdir=RT;
                                    else if (rand%8==4) newdir=Top;
                                    else if (rand%8==5) newdir=TL;
                                    else if (rand%8==6) newdir=Left;
                                    else if (rand%8==7) newdir=LB;
                                }
                                visited[newpos.i][newpos.j]=true;
                                qu.push(make_pair(newpos,newdir));
                            }
                        }
                        primera=false;
                    }
                }
            }
            else{
                bool found=false;
                while (not qu.empty() and not found){
                    Pos act_pos=qu.front().first;
                    Dir act_dir=qu.front().second; qu.pop();
                    Cell c=cell(act_pos);
                    newdir=act_dir;
                    if ((c.id!=-1 and unit(c.id).player!=me()) or (c.owner!=me() and c.type==Cave) or (c.treasure)){
                        found=true;
                        command(id, act_dir);
                    }
                    else { //mirar veins
                        int rand=random(0,8);
                        for (int p=0; p<8 and not found; ++p){
                            Pos newpos;
                            ++rand;
                            newpos=act_pos+Dir(rand%8);
                            if (posicion_valida_dwarf(newpos,visited)){
                                if (primera) {
                                    if (rand%8==0) newdir=Bottom;
                                    else if (rand%8==1) newdir=BR;
                                    else if (rand%8==2) newdir=Right;
                                    else if (rand%8==3) newdir=RT;
                                    else if (rand%8==4) newdir=Top;
                                    else if (rand%8==5) newdir=TL;
                                    else if (rand%8==6) newdir=Left;
                                    else if (rand%8==7) newdir=LB;
                                }
                                visited[newpos.i][newpos.j]=true;
                                qu.push(make_pair(newpos,newdir));
                            }
                        }
                        primera=false;
                    }
                }
            }
        }
    }
    
    void move_wizards(){
        vector<int> W = wizards(me());
        int n=W.size();
        for (int r=0; r<n; ++r){
            int id=W[r];
            Pos inicial=unit(id).pos;
            vector<vector<bool>> visited(60,vector<bool>(60,false));
            visited[inicial.i][inicial.j]=true;
            queue<pair<Pos,Dir>> qu;
            Dir newdir=None;
            qu.push(make_pair(inicial,newdir));
            bool found=false;
            while (not qu.empty() and not found){
                Pos act_pos=qu.front().first;
                Dir act_dir=qu.front().second; qu.pop();
                Cell c=cell(act_pos);
                newdir=act_dir;
                if (c.id!=-1 and unit(c.id).type == Dwarf) {
                    found=true;
                    command(id,newdir);
                }
                else { //mirar veins
                    for (int p=0; p<4; ++p){
                        Pos newpos;
                        newpos=act_pos+Dir(p*2);
                        if (posicion_valida_wizard(newpos,visited)){
                            if (act_dir==None) {
                                if (p==0) newdir=Top;
                                else if (p==1) newdir=Right;
                                else if (p==2) newdir=Top;
                                else if (p==3) newdir=Left;
                            }
                            visited[newpos.i][newpos.j]=true;
                            qu.push(make_pair(newpos,newdir));
                        }
                    }
                }
            }             
        }
    }
    
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
      move_dwarves();
      //move_wizards();
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
