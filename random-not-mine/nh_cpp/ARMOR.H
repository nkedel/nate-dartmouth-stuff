class armorclass: public objectclass{
protected:        

        schar enchantment;
        Bitfields(rustproof,1);
        Bitfields(rustproofknown,1);
        Bitfields(enchantmentknown,1);
        Bitfields(rusted,2);            // status of rust
        uchar baseac;                    // compute by (10-baseac)
                                        // Really needs 4 bits (up to -4)
public:
        void rust() {if ( ( !rustproof ) && ( (int)rusted < 3 ) ) ((int)rusted)++;}
        uchar isrusted() {return (uchar)rusted;}
        void enchant(){cout << "It glows."<<endl;}
        void draw();
        uchar isarmor(){return(TRUE);}
        uchar getclass(){return(ARMOR_CLASS);}
};

