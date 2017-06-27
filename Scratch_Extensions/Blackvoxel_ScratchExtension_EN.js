/*
 * 
 * 
 * Blackvoxel Scratch Extension.
 * 
 * 
 * 
 */
 
 
(function(ext) {
	
	// URL and Port used for communication with blackvoxel
	
	ext.ip =  '127.0.0.1';
	ext.port= '48559';
	
    ext._shutdown = function() {};

    ext._getStatus = function() {
        return {status: 2, msg: 'Ready'};
    };

    // Go home, reset robot position
       
    ext.do_home = function(callback) {
        $.ajax({
              url: 'http://'+ext.ip+':'+ext.port+'/do_home',
              dataType: 'jsonp',
              success: function( status ) {
                  callback();
              }
        });
    };
    
    
    
    ext.do_dig = function(DigDirection,callback) {
		switch(DigDirection)
		{
		  case 'Ahead':     Dir=0; break;
		  case 'Behind':    Dir=2; break;
		  case 'Above':     Dir=4; break;
		  case 'Below':     Dir=5; break;
		  case 'Left':      Dir=3; break;
		  case 'Right':     Dir=1; break;
		  default:          Dir=0; break;
		}
        $.ajax({
              url: 'http://'+ext.ip+':'+ext.port+'/do_dig?dir='+Dir,
              dataType: 'jsonp',
              success: function( status ) {
                  callback();
              }
        });
    }; 

    ext.do_move = function(MoveDirection,callback) {
		switch(MoveDirection)
		{
		  case 'Forward':     Dir=0; break;
		  case 'Backward':    Dir=2; break;
		  case 'Up':          Dir=4; break;
		  case 'Down':        Dir=5; break;
		  default:            Dir=0; break;
		}
        $.ajax({
              url: 'http://'+ext.ip+':'+ext.port+'/do_move?dir='+Dir,
              dataType: 'jsonp',
              success: function( status ) {
                  callback();
              }
        });
    }; 
    
    ext.do_turn = function(MoveDirection,callback) {
		switch(MoveDirection)
		{
		  case 'Right':      Dir=0; break;
		  case 'Left':       Dir=1; break;
		  default:           Dir=0; break;
		}
        $.ajax({
              url: 'http://'+ext.ip+':'+ext.port+'/do_turn?dir='+Dir,
              dataType: 'jsonp',
              success: function( status ) {
                  callback();
              }
        });
    };

    ext.do_place = function(PlaceDirection,callback) {
		switch(PlaceDirection)
		{
		  case 'Ahead':     Dir=0; break;
		  case 'Behind':    Dir=2; break;
		  case 'Above':     Dir=4; break;
		  case 'Below':     Dir=5; break;
		  case 'Left':      Dir=3; break;
		  case 'Right':     Dir=1; break;
		  default:          Dir=0; break;
		}
        $.ajax({
              url: 'http://'+ext.ip+':'+ext.port+'/do_place?dir='+Dir,
              dataType: 'jsonp',
              success: function( status ) {
                  callback();
              }
        });
    }; 
    
    ext.get_coord = function(CoordType, callback) {
        switch(CoordType)
        {
	      case 'X':     Info = 0; break;
	      case 'Y':     Info = 1; break;
	      case 'Z':     Info = 2; break;
	      case 'Abs X': Info = 3; break;
	      case 'Abs Y': Info = 4; break;
	      case 'Abs Z': Info = 5; break;
	      default:      Info = 0; break;
		}

        $.ajax({
              url: 'http://'+ext.ip+':'+ext.port+'/get_coord?info='+Info,
              dataType: 'jsonp',
              success: function( ret_data ) {
                  // Got the data - parse it and return the temperature
                  Coord = ret_data['main']['data'];
                  callback(Coord);
              }
        });
    };
    
    ext.get_look = function(LookDirection, callback) {
		switch(LookDirection)
		{
		  case 'Ahead':     Dir=0; break;
		  case 'Behind':    Dir=2; break;
		  case 'Above':     Dir=4; break;
		  case 'Below':     Dir=5; break;
		  case 'Left':      Dir=3; break;
		  case 'Right':     Dir=1; break;
		  default:          Dir=0; break;
		}

        $.ajax({
              url: 'http://'+ext.ip+':'+ext.port+'/get_look?dir='+Dir,
              dataType: 'jsonp',
              success: function( ret_data ) {
                  // Got the data - parse it and return the temperature
                  Coord = ret_data['main']['data'];
                  callback(Coord);
              }
        });
    };

    ext.get_invquantity = function(VoxelType, InventoryType, callback) {
		
		switch(InventoryType)
		{
		  case 'Robot':      Inv=6; break;
		  default:           Inv=6; break;
		}

        $.ajax({
              url: 'http://'+ext.ip+':'+ext.port+'/get_invquantity?inv='+Inv+'&voxeltype='+VoxelType,
              dataType: 'jsonp',
              success: function( ret_data ) {
                         Qtty = ret_data['main']['data'];
                         console.log(Qtty);
                         callback(Qtty);
                       }
        });
    };

    ext.do_push = function(VoxelType, PushDirection, callback) {
		
		switch(PushDirection)
		{
		  case 'Ahead':     Dir=0; break;
		  case 'Behind':    Dir=2; break;
		  case 'Above':     Dir=4; break;
		  case 'Below':     Dir=5; break;
		  case 'Left':      Dir=3; break;
		  case 'Right':     Dir=1; break;
		  default:          Dir=0; break;
		}
        $.ajax({
              url: 'http://'+ext.ip+':'+ext.port+'/do_push?dir='+Dir+'&voxeltype='+VoxelType,
              dataType: 'jsonp',
              success: function( status ) {
                  callback();
              }
        });
    };

    ext.do_pull = function(PullDirection, callback) {
		switch(PullDirection)
		{
		  case 'Ahead':     Dir=0; break;
		  case 'Behind':    Dir=2; break;
		  case 'Above':     Dir=4; break;
		  case 'Below':     Dir=5; break;
		  case 'Left':      Dir=3; break;
		  case 'Right':     Dir=1; break;
		  default:          Dir=0; break;
		}
        $.ajax({
              url: 'http://'+ext.ip+':'+ext.port+'/do_pull?dir='+Dir,
              dataType: 'jsonp',
              success: function( status ) {
                  callback();
              }
        });
    };


    ext.do_voxeltype = function(VoxelType, callback) {
        $.ajax({
              url: 'http://'+ext.ip+':'+ext.port+'/do_voxeltype?voxeltype='+VoxelType,
              dataType: 'jsonp',
              success: function( status ) {
                  callback();
              }
        });
    };
    
    ext.do_special = function(Fnct, Par1, Par2, callback) 
    {
      switch(Fnct)
      {
        case 'IP': ext.ip = Par1;  
                   if (Par2 != 0) ext.port = Par2;
                   callback();
                   break;

        case 'Strict Mode':
                            $.ajax({
                                     url: 'http://'+ext.ip+':'+ext.port+'/do_special_setstrictmode?setting='+Par1,
                                     dataType: 'jsonp',
                                     success: function( status ) { callback() }
                                   });
                             break;

        default:   callback();
                   break;           
      }
    };
    
    ext.get_special = function(Fnct, Par1, Par2, callback) 
    {
      switch(Fnct)
      {
		case 'Nombre Cases Inventaire':
              $.ajax({
                       url: 'http://'+ext.ip+':'+ext.port+'/get_special_invslotcount',
                       dataType: 'jsonp',
                       success: function( ret_data ) { Cases = ret_data['main']['data']; callback(Cases); }
                     });
              break;
        
        case 'Voxel Type in slot':
              $.ajax({
                       url: 'http://'+ext.ip+':'+ext.port+'/get_special_invslotvoxeltype?slot='+Par1,
                       dataType: 'jsonp',
                       success: function( ret_data ) { Cases = ret_data['main']['data']; callback(Cases); }
                     });
              break;
        case 'Voxel Count in Slot':
              $.ajax({
                       url: 'http://'+ext.ip+':'+ext.port+'/get_special_invslotvoxelqtty?slot='+Par1,
                       dataType: 'jsonp',
                       success: function( ret_data ) { Qtty = ret_data['main']['data']; callback(Qtty); }
                     });              
              break;
        default: callback(0);
      }
    };
     
    
    // Block and block menu descriptions
    var descriptor = {
        blocks: [
            ['w', 'Home', 'do_home'],
            ['w', 'Move %m.MoveMenu', 'do_move', 'Forward'],
            ['w', 'Turn %m.TurnMenu', 'do_turn', 'Right'],
            ['w', 'Dig %m.LocMenu ', 'do_dig', 'Ahead'],
            ['w', 'Use Cube Type %n', 'do_voxeltype', '3'],
            ['w', 'Place %m.LocMenu ', 'do_place', 'Ahead'],
            ['R', 'Which Bloc %m.LocMenu ?', 'get_look','Ahead'],
            ['R', 'Location %m.CoordMenu', 'get_coord', 'x'],
            ['R', 'Qnt of cube %n dans %m.InventoryMenu', 'get_invquantity','1','Robot'],
            ['w', 'Export %n %m.LocMenu ', 'do_push', '1', 'Ahead'],
            ['w', 'Import %m.LocMenu ', 'do_pull', 'Ahead'],
            ['w', 'Advanced : %m.MiscCommandMenu %s : %s ', 'do_special', '', 0, 0],
            ['R', 'Advanced : %m.MiscInfoMenu %s : %s', 'get_special','','0','0']
        ],
        menus: {
			MoveMenu:['Forward', 'Backward','Up', 'Down'],
			TurnMenu:['Right', 'Left'],
			LocMenu: ['Ahead', 'Behind','Above', 'Below', 'Left', 'Right'],
			CoordMenu: ['X','Y','Z', 'Abs X', 'Abs Y', 'Abs Z'],
			InventoryMenu: ['Robot'],
			MiscCommandMenu: ['IP', 'Strict Mode'],
			MiscInfoMenu: ['Inventory Slot Count', 'Voxel Type in slot', 'Voxel Count in Slot']
		}
    };

    // Register the extension
    ScratchExtensions.register('Blackvoxel extension', descriptor, ext);
})({});
