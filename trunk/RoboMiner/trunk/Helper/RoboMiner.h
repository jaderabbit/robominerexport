namespace RoboMiner {
	
	enum { EMPTY, GOLD, WASTE, ROBOT, SINK, G_SINK, W_SINK };

	enum { CLUSTER, FORAGE, EXPLORE, BASICFORAGE };


	enum { HOMING, BEACON_HOMING, CHOOSE_ACTIVITY, 			//General States
		   RECRUITING, EXPLORING,					//Explorer States
		   WAITING, LOCATING, LOCAL_CLUSTER_SEARCH, LOADING, UNLOADING,	//Forager States
		   UNLOADED, LOADED						//Ant Cemetary States
		  };
}