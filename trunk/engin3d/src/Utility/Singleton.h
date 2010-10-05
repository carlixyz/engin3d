#ifndef SINGLETON_H
#define	SINGLETON_H

//Singleton class
template<typename T> class cSingleton
{
  public:
    static T& Get()
    {
      //Return the static reference to itself.
      //To be used instead the constructor.
      static T lInstance;
      return lInstance;
    }
};

#endif	

