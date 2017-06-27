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
		  case 'Devant':     Dir=0; break;
		  case 'Derriere':   Dir=2; break;
		  case 'Au Dessus':  Dir=4; break;
		  case 'En Dessous': Dir=5; break;
		  case 'A Gauche':   Dir=3; break;
		  case 'A Droite':   Dir=1; break;
		  default:           Dir=0; break;
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
		  case 'Avancer':     Dir=0; break;
		  case 'Reculer':     Dir=2; break;
		  case 'Monter':      Dir=4; break;
		  case 'Descendre':   Dir=5; break;
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
		  case 'A Droite':     Dir=0; break;
		  case 'A Gauche':     Dir=1; break;
		  default:             Dir=0; break;
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
		  case 'Devant':     Dir=0; break;
		  case 'Derriere':   Dir=2; break;
		  case 'Au Dessus':  Dir=4; break;
		  case 'En Dessous': Dir=5; break;
		  case 'A Gauche':   Dir=3; break;
		  case 'A Droite':   Dir=1; break;
		  default:           Dir=0; break;
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
		  case 'Devant':     Dir=0; break;
		  case 'Derriere':   Dir=2; break;
		  case 'Au Dessus':  Dir=4; break;
		  case 'En Dessous': Dir=5; break;
		  case 'A Gauche':   Dir=3; break;
		  case 'A Droite':   Dir=1; break;
		  default:           Dir=0; break;
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
		  case 'Devant':     Dir=0; break;
		  case 'Derriere':   Dir=2; break;
		  case 'Au Dessus':  Dir=4; break;
		  case 'En Dessous': Dir=5; break;
		  case 'A Gauche':   Dir=3; break;
		  case 'A Droite':   Dir=1; break;
		  default:           Dir=0; break;
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
		  case 'Devant':     Dir=0; break;
		  case 'Derriere':   Dir=2; break;
		  case 'Au Dessus':  Dir=4; break;
		  case 'En Dessous': Dir=5; break;
		  case 'A Gauche':   Dir=3; break;
		  case 'A Droite':   Dir=1; break;
		  default:           Dir=0; break;
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

        case 'Mode Strict':
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
        
        case 'VoxelType dans Slot':
              $.ajax({
                       url: 'http://'+ext.ip+':'+ext.port+'/get_special_invslotvoxeltype?slot='+Par1,
                       dataType: 'jsonp',
                       success: function( ret_data ) { Cases = ret_data['main']['data']; callback(Cases); }
                     });
              break;
        case 'Voxels dans slot':
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
            ['w', 'Retour Origine', 'do_home'],
            ['w', 'Deplacer %m.MoveMenu', 'do_move', 'Avancer'],
            ['w', 'Tourner %m.TurnMenu', 'do_turn', 'A Droite'],
            ['w', 'Creuser %m.LocMenu ', 'do_dig', 'Devant'],
            ['w', 'Utiliser Cube Type %n', 'do_voxeltype', '3'],
            ['w', 'Placer %m.LocMenu ', 'do_place', 'Devant'],
            ['R', 'Quel Bloc %m.LocMenu ?', 'get_look','Devant'],
            ['R', 'Position %m.CoordMenu', 'get_coord', 'x'],
            ['R', 'Qnt de Cube %n dans %m.InventoryMenu', 'get_invquantity','1','Robot'],
            ['w', 'Exporter %n %m.LocMenu ', 'do_push', '1', 'Devant'],
            ['w', 'Importer %m.LocMenu ', 'do_pull', 'Devant'],
            ['w', 'Avancé : %m.MiscCommandMenu %s : %s ', 'do_special', '', 0, 0],
            ['R', 'Avancé : %m.MiscInfoMenu %s : %s', 'get_special','','0','0']
        ],
        menus: {
			MoveMenu:['Avancer', 'Reculer','Monter', 'Descendre'],
			TurnMenu:['A Droite', 'A Gauche'],
			LocMenu: ['Devant', 'Derriere','Au Dessus', 'En Dessous', 'A Gauche', 'A Droite'],
			CoordMenu: ['X','Y','Z', 'Abs X', 'Abs Y', 'Abs Z'],
			InventoryMenu: ['Robot'],
			MiscCommandMenu: ['IP', 'Mode Strict'],
			MiscInfoMenu: ['Nombre Cases Inventaire', 'VoxelType dans Slot', 'Voxels dans slot']
		}
    };

    // Register the extension
    ScratchExtensions.register('Blackvoxel extension', descriptor, ext);
})({});
