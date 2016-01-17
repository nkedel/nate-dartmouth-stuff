class weaponclass :public objectclass{
protected:
        schar enchantment;
        Bitfields(rustproof,1);
        Bitfields(rustproofknown,1);
        Bitfields(enchantmentknown,1);
        Bitfields(twohanded,1);
        Bitfields(rusted,2);            // status of rust
        Bitfields(unused,2);
public:
        void rust() {if ( ( !rustproof ) && ( (int)rusted < 3 ) ) ((int)rusted)++;}
        uchar isrusted() {return (uchar)rusted;}
        void enchant(){cout << "It glows" <<endl;}
        void draw();
        uchar isweapon(){return(TRUE);}
        uchar getclass(){return(WEAPON_CLASS);}
        void wield(monstclass *mon){}
        void unwield(monstclass *mon){}
        void drop(monstclass *mon){}
};

