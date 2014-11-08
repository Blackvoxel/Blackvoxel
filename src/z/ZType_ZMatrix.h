

#ifndef Z_ZTYPE_ZMATRIX_H
#define Z_ZTYPE_ZMATRIX_H

// #ifndef Z_ZTYPE_ZVECTOR3D_H
// #  include "ZType_ZVector3d.h"
// #endif

#ifndef Z_ZTYPES_H
#  include "ZTypes.h"
#endif

class ZMatrix
{
#define vRight 0
#define vUp 1
#define vForward 2
#define DOFUNC(n) n

	int nTime;
	double s[4];
	double q[4];
	double gl_m[4][4];

public:
	double m[4][4];

	ZMatrix()
	{
		s[0] = 1;
		s[1] = 1;
		s[2] = 1;
		s[3] = 1;
		memset( m, 0, sizeof( m ) );
		m[0][0] = 1;
		m[1][1] = 1;
		m[2][2] = 1;
		m[3][3] = 1;
	}

	inline double pitch( void )
	{
		if( ( m[2][2] > 0.707 ) )
			return atan2( m[2][1], m[2][2] ) * (360 / (2*3.14159)); 
		else if( ( m[2][2] < -0.707 ) )
			return atan2( m[2][1], -m[2][2] ) * (360 / (2*3.14159)); 
		else
			if( m[2][0] < 0 )
				return atan2( m[2][1], -m[2][0] ) * (360 / (2*3.14159)); 
			else
				return atan2( m[2][1], m[2][0] ) * (360 / (2*3.14159)); 


		return atan2( -m[2][1], sqrt( 1-m[2][1] * m[2][1]) ) * (360 / (2*3.14159)); 
		// sohcah toa  
		return asin( m[0][2] ) * (360 / (2*3.14159));
	}
	inline double yaw( void )
	{
		if( m[2][1] < 0.707 && m[2][1] > -0.707 )
		{
			return atan2( m[2][0], m[2][2] ) * (360 / (2*3.14159)); 
		}
		else
		{
			if( m[1][2] < 0 )
				return atan2( m[1][0], m[1][2] ) * (360 / (2*3.14159)); 
			else
				return atan2( m[1][0], m[1][2] ) * (360 / (2*3.14159)); 
		}
		// sohcah toa  
		//return atan2( m[0][1], m[0][0] ) * (360 / (2*3.14159)); 
		return asin( m[0][2] ) * (360 / (2*3.14159));
	}

	inline double roll( void )
	{
		// sohcah toa  
		if( ( m[0][0] > 0.707 ) )
			return atan2( m[0][1], m[0][0] ) * (360 / (2*3.14159)); 
		else if( ( m[0][0] < -0.707 ) )
			return atan2( m[0][1], -m[0][0] ) * (360 / (2*3.14159)); 
		else
			if( m[0][2] < 0 )
				return atan2( m[0][1], -m[0][2] ) * (360 / (2*3.14159)); 
			else
				return atan2( m[0][1], m[0][2] ) * (360 / (2*3.14159)); 

		return asin( m[0][1] ) * (360 / (2*3.14159));
	}

	inline double * x_axis( void )
	{
		return m[0];
	}
	inline ZVector3d &y_axis( void )
	{
		return (ZVector3d &)m[1];
	}
	/*
	inline double * z_axis( void )
	{
		return m[2];
	}
	*/
	inline ZVector3d & z_axis( void )
	{
		return (ZVector3d &)m[2];
	}

	inline ZVector3d _2d_forward( void )
	{
		ZVector3d result = m[vForward];
		double len = sqrt( result.x * result.x + result.z * result.z );
		if( len > 0.001 )
		{
			result.x = result.x / len;
			result.y = 0;
			result.z = result.z / len;
		}
		else
		{
			result.x = 0;
			result.y = 0;
			result.z = 0;
		}
		return result;
	}

	inline ZVector3d _2d_left( void )
	{
		ZVector3d result = _2d_forward();
		double tmp;
		tmp = result.x;
		result.x = result.z;
		result.z = -tmp;
		return result;
	}
	inline void ApplyRotation( ZVector3d &dest, ZVector3d &src )
	{
	   #define i 0
	   dest.x = s[i] * ( m[vRight]  [i] * src.x +
				 m[vUp]     [i] * src.y +
				 m[vForward][i] * src.z );
	   #undef i
	   #define i 1
	   dest.y = s[i] * ( m[vRight]  [i] * src.x +
				 m[vUp]     [i] * src.y +
				 m[vForward][i] * src.z );
	   #undef i
	   #define i 2
	   dest.z = s[i] * ( m[vRight]  [i] * src.x +
				 m[vUp]     [i] * src.y +
				 m[vForward][i] * src.z );
	   #undef i
	}

	
		inline void RotateAround( ZVector3d p, double amount )
		{
			// P defines an axis around which the rotation portion of the matrix
			// is rotated by an amount.
			// coded from http://www.mines.edu/~gmurray/ArbitraryAxisRotation/ArbitraryAxisRotation.html *dead*
			// and http://www.siggraph.org/education/materials/HyperGraph/modeling/mod_tran/3drota.htm
		   // and http://astronomy.swin.edu.au/~pbourke/geometry/rotate/ *dead*
			//  *** Really not sure if any of the prior things matter? since it's translated into a matrix 
			//      and just applied?  Or is the building of the matrix the prior links? ***
			ZMatrix t;  // temp matrix
			//ZMatrix *T = this;
			double Len = p.Length( );
			double Cos = cos(amount * (2*3.14159268/360));
			double Sin = sin(amount * (2*3.14159268/360));
			double normal;
			t.s[0] = 1;
			t.s[1] = 1;
			t.s[2] = 1;
			// actually the only parts of the matrix resulting
			// will only be the rotation matrix, for which we are
			// building an absolute translation... which may be saved by
			// passing an identity filled transform... but anyhow...
			// the noise in the speed, accel, etc resulting from uninitialized
			// stack space being used for the transform this is building, matters
		   // not at all.
			//pt = &t;
			//SetPoint( _v, p );
			//normalize( _v );
		#define u p[0]
		#define v p[1]
		#define up p[2]
			// okay this is rude and ugly, and could be optimized a bit
		   // but we do have a short stack and 3 are already gone.
		   normal = u*u+v*v+up*up;
			t.m[0][0] = u * u + ( v * v + up * up ) * Cos
			  / normal;
			t.m[0][1] = u*v * ( 1-Cos ) - up * Len * Sin
			  / normal;
			t.m[0][2] = u*up*(1-Cos) + v*Len * Sin
			  / normal;
			t.m[1][0] = u*v*(1-Cos) + up*Len * Sin
			  / normal;
			t.m[1][1] = v*v + (u*u+up*up)*Cos
			  / normal;
			t.m[1][2] = v*up*(1-Cos)-u*Len*Sin
			  / normal;
			t.m[2][0] = u*up*(1-Cos)-v*Len*Sin
			  / normal;
			t.m[2][1] = v*up*(1-Cos)+u*Len*Sin
			  / normal;
			t.m[2][2] = up*up+(u*u + v*v)*Cos
			  / normal;
			// oh yeah , be nice, and release these symbols...
			// V is such a common vector variable :)
		#undef u
		#undef v
		#undef up

			t.ApplyRotation( *this, *this );
		}

	inline void ApplyRotation( double *dest, double *src )
	{
	   #define i 0
	   dest[i] = s[i] * ( m[vRight]  [i] * src[0] +
				 m[vUp]     [i] * src[1] +
				 m[vForward][i] * src[2] );
	   #undef i
	   #define i 1
	   dest[i] = s[i] * ( m[vRight]  [i] * src[0] +
				 m[vUp]     [i] * src[1] +
				 m[vForward][i] * src[2] );
	   #undef i
	   #define i 2
	   dest[i] = s[i] * ( m[vRight]  [i] * src[0] +
				 m[vUp]     [i] * src[1] +
				 m[vForward][i] * src[2] );
	   #undef i
	}

	inline void Apply( double *v_out, double *v_in )
	{
		ApplyRotation( v_out, v_in );
		add3( v_out, v_in, m[3] );
	}

	inline void Apply( ZVector3d &v_out, ZVector3d &v_in )
	{
		ApplyRotation( v_out, v_in );
		add3( v_out, v_in, m[3] );
	}

	inline void Apply( ZVector3d &v )
	{
		ZVector3d out;
		Apply( out, v );
		v = out;
	}

	inline void ApplyRotation( ZMatrix &m_out, ZMatrix &m_in )
	{
		ZMatrix t;

		ZVector3d out;
		ApplyRotation( t.m[0], m_in.m[0] );
		ApplyRotation( t.m[1], m_in.m[1] );
		ApplyRotation( t.m[2], m_in.m[2] );
		memcpy( t.m[3], m_in.m[3], sizeof( double ) * 3 );

		m_out = t;
	}
	inline void Apply( ZMatrix &m_out, ZMatrix &m_in )
	{
		ZMatrix t;

		ZVector3d out;
		ApplyRotation( t.m[0], m_in.m[0] );
		ApplyRotation( t.m[1], m_in.m[1] );
		ApplyRotation( t.m[2], m_in.m[2] );
		Apply( t.m[3], m_in.m[3] );

		m_out = t;
	}
	inline void translate( double x, double y, double z )
	{
		m[3][0] = x;
		m[3][1] = y;
		m[3][2] = z;
	}
	inline void translate_rel( double x, double y, double z )
	{
		m[3][0] += x;
		m[3][1] += y;
		m[3][2] += z;
	}
	inline void translate( double *v )
	{
		m[3][0] = v[0];
		m[3][1] = v[1];
		m[3][2] = v[2];
	}
	inline void translate_rel( double *v )
	{
		m[3][0] += v[0];
		m[3][1] += v[1];
		m[3][2] += v[2];
	}
	inline void quat( double *quat )
	{

		m[0][0] = 1 - 2 * quat[2] * quat[2] - 2 * quat[3] * quat[3];
		m[0][1] = 2 * quat[1] * quat[2] - 2 * quat[3] * quat[0];
		m[0][2] = (2 * quat[1] * quat[3] + 2 * quat[2] * quat[0]);
		m[1][0] = 2 * quat[1] * quat[2] + 2 * quat[3] * quat[0];
		m[1][1] = 1 - 2 * quat[1] * quat[1] - 2 * quat[3] * quat[3];
		m[1][2] = (2 * quat[2] * quat[3] - 2 * quat[1] * quat[0]);
		m[2][0] = 2 * quat[1] * quat[3] - 2 * quat[2] * quat[0];
		m[2][1] = 2 * quat[2] * quat[3] + 2 * quat[1] * quat[0];
		m[2][2] = (1 - 2 * quat[1] * quat[1] - 2 * quat[2] * quat[2]);

	}
	inline double *quat( void )
	{
	//	t = Qxx+Qyy+Qzz (trace of Q)
	//r = sqrt(1+t)
	//w = 0.5*r
	//x = copysign(0.5*sqrt(1+Qxx-Qyy-Qzz), Qzy-Qyz)
	//y = copysign(0.5*sqrt(1-Qxx+Qyy-Qzz), Qxz-Qzx)
	//z = copysign(0.5*sqrt(1-Qxx-Qyy+Qzz), Qyx-Qxy)
		/*
		where copysign(x,y) is x with the sign of y:
		copysign(x,y) = sign(y) |x|;

		*/
		/*
		t = Qxx+Qyy+Qzz
	r = sqrt(1+t)
	s = 0.5/r
	w = 0.5*r
	x = (Qzy-Qyz)*s
	y = (Qxz-Qzx)*s
	z = (Qyx-Qxy)*s
	*/
		double t = m[0][0] + m[1][1] + m[2][2];
		double r = sqrt(1+t);
		double s = ((double)0.5)/r;
		q[0] = ((double)0.5)*r;
		q[1] = (m[2][1]-m[1][2])*s;
		q[2] = (m[0][2]-m[2][0])*s;
		q[3] = (m[1][0]-m[0][1])*s;

		return q;
	}


private:
	inline void scale( double v_out[4], double v_in[4], double k )
	{
		v_out[0] = v_in[0] * k;
		v_out[1] = v_in[1] * k;
		v_out[2] = v_in[2] * k;
		v_out[3] = v_in[3] * k;
	}

	inline void add( double v_out[4], double v_in[4], double v_in2[4] )
	{
		v_out[0] = v_in[0] + v_in2[0];
		v_out[1] = v_in[1] + v_in2[1];
		v_out[2] = v_in[2] + v_in2[2];
		v_out[3] = v_in[3] + v_in2[3];
	}
	inline void add3( double v_out[4], double v_in[4], double v_in2[4] )
	{
		v_out[0] = v_in[0] + v_in2[0];
		v_out[1] = v_in[1] + v_in2[1];
		v_out[2] = v_in[2] + v_in2[2];
		//v_out[3] = v_in[3] + v_in2[3];
	}
	inline void sub( double v_out[4], double v_in[4], double v_in2[4] )
	{
		v_out[0] = v_in[0] - v_in2[0];
		v_out[1] = v_in[1] - v_in2[1];
		v_out[2] = v_in[2] - v_in2[2];
		v_out[3] = v_in[3] - v_in2[3];
	}

 void Rotate ( double dAngle, double vaxis1[4], double vaxis2[4] )
{
   double v1[4], v2[4];
   double vsave[4];
   double dsin = sin( dAngle )
   	  , dcos = cos( dAngle );
   memcpy( vsave, vaxis1, sizeof( vsave ) );
    scale( v1, vaxis1, dcos );
    scale( v2, vaxis2, dsin );
   sub( vaxis1, v1, v2 );
   scale( v1, vsave, dsin );
   scale( v2, vaxis2, dcos );
   add( vaxis2, v2, v1 );
}

public:
	inline void ApplyInverseRotation( ZVector3d *dest, ZVector3d *src )
	{
		#define i 0
		dest->x = m[i][vRight]   * src->x +
				 m[i][vUp]      * src->y +
				 m[i][vForward] * src->z;
		#undef i
		#define i 1
		dest->y = m[i][vRight]   * src->x +
				 m[i][vUp]      * src->y +
				 m[i][vForward] * src->z;
		#undef i
		#define i 2
		dest->z = m[i][vRight]   * src->x +
				 m[i][vUp]      * src->y +
				 m[i][vForward] * src->z;
		#undef i
	}
	inline void ApplyInverseRotation( ZVector3d &dest, ZVector3d &src )
	{
		#define i 0
		dest.x = m[i][vRight]   * src.x +
				 m[i][vUp]      * src.y +
				 m[i][vForward] * src.z;
		#undef i
		#define i 1
		dest.y = m[i][vRight]   * src.x +
				 m[i][vUp]      * src.y +
				 m[i][vForward] * src.z;
		#undef i
		#define i 2
		dest.z = m[i][vRight]   * src.x +
				 m[i][vUp]      * src.y +
				 m[i][vForward] * src.z;
		#undef i
	}
private:
	inline void Invert( double v[3] )
	{
		v[0] = -v[0];
		v[1] = -v[1];
		v[2] = -v[2];
	}

public:

	double *glMatrix( void )
	{
		
		// ugly but perhaps there will be some optimization if I
		// do this linear like... sure it's a lot of code, but at
		// least there's no work to loop and multiply...
		gl_m[0][0] = m[0][0];
		gl_m[0][1] = m[1][0];
		gl_m[0][2] = m[2][0];
		//gl_m[0][3] = m[3][0];
		gl_m[0][3] = m[0][3];

		gl_m[1][0] = m[0][1];
		gl_m[1][1] = m[1][1];
		gl_m[1][2] = m[2][1];
		//gl_m[1][3] = m[3][1];
		gl_m[1][3] = m[1][3];

		// z was inverted of what it should have been...
		gl_m[2][0] = m[0][2];
		gl_m[2][1] = m[1][2];
		gl_m[2][2] = m[2][2];
		//gl_m[2][3] = m[3][2];
		gl_m[2][3] = m[2][3];

		//gl_m[3][0] = m[0][3];
		//gl_m[3][1] = m[1][3];
		//gl_m[3][2] = m[2][3];
		//gl_m[3][3] = m[3][3];
		 // okay apparently opengl applies
		 // this origin, and then rotates according to the
		 // above matrix... so I need to undo having the correct
		// bias on the translation.
		 //DOFUNC(ApplyInverseRotation)( pt, gl_m[3], m[3] );
		//Invert( m[2] );
		DOFUNC(ApplyInverseRotation)( (ZVector3d*)gl_m[3], (ZVector3d*)m[3] );
		//Invert( m[2] );
		Invert( gl_m[3] );

		gl_m[3][3] = m[3][3];
		return (double*)gl_m;
	}



	inline double &x()
	{
		return m[3][0];
	}
	inline double &y()
	{
		return m[3][1];
	}
	inline double &z()
	{
		return m[3][2];
	}

	inline void RotateYaw(double a) {
		if( a ) Rotate( a * (2*3.14159/360),m[vRight],m[vForward] );
	}
	inline void RotatePitch(double a) {
		if( a ) Rotate( a * (2*3.14159/360),m[vForward],m[vUp] );
	}
	inline void RotateRoll(double a) {
		if( a ) Rotate( a * (2*3.14159/360),m[vUp],m[vRight] );
	}

#define _RotateYaw(a)        if(a) Rotate( a*(2*3.14159268/360),m[vRight],m[vForward] )
#define _RotatePitch(a)      if(a) Rotate( a*(2*3.14159268/360),m[vForward],m[vUp] )
#define _RotateRoll(a)       if(a) Rotate( a*(2*3.14159268/360),m[vUp],m[vRight] )

	inline void get_origin(ZVector3d &v)
	{
		v.x = m[3][0];
		v.y = m[3][1];
		v.z = m[3][2];
	}

	inline ZVector3d &origin()
	{
		return (ZVector3d&)m[3];
	}
	void RotateAbsolute( double yaw, double pitch, double roll )
	{
		   double rcos[3]; // cos(rx), cos(ry), cos(rz)
			double rcosf[3]; // cos[2]*cos[1], cos[2]*cos[0], cos[1]*cos[0]
			yaw *= 2*3.14159268/360.0;
			pitch *= 2*3.14159268/360.0;
			roll *= 2*3.14159268/360.0;
		   m[1][0] = -(m[0][1] = sin(roll));
		   m[2][0] = -(m[0][2] = sin(yaw));
		   m[2][1] = -(m[1][2] = sin(pitch));
		   m[0][0] = //s[0] *  // scale???? ookay...
			  ( rcosf[0] = ( rcos[2] = cos(roll) ) 
						 * ( rcos[1] = cos(yaw) ) );
		   m[1][1] = //s[1] *
			  ( rcosf[1] = ( rcos[2] ) * ( rcos[0] = cos( pitch ) ) );
		   m[2][2] = //s[2] *
			  ( rcosf[2] = ( rcos[1] ) * ( rcos[0] ) );

	}
	void Rotate( double yaw, double pitch, double roll )
	{ // depends on Scale function....
	   //if( !motion )
		//   CreateTransformMotion( pt );
	   switch( nTime++ )
	   {
	   case 0:
		  _RotateYaw   ( yaw );
		  _RotatePitch ( pitch );
		  _RotateRoll  ( roll );
		  break;
	   case 1:
		  _RotateYaw   ( yaw );
		  _RotateRoll  ( roll );
		  _RotatePitch ( pitch );
		  break;
	   case 2:
		  _RotatePitch ( pitch );
		  _RotateYaw   ( yaw );
		  _RotateRoll  ( roll );
		  break;
	   case 3:
		  _RotatePitch ( pitch );
		  _RotateRoll  ( roll );
		  _RotateYaw   ( yaw );
		  break;
	   case 4:
		  _RotateRoll  ( roll );
		  _RotatePitch ( pitch );
		  _RotateYaw   ( yaw );
		  break;
	   default:
		  nTime = 0;
		  _RotateRoll  ( roll );
		  _RotateYaw   ( yaw );
		  _RotatePitch ( pitch );
		  break;
	   }
	}

	inline void Apply( ZMatrix matrix )
	{
	}

};



#endif