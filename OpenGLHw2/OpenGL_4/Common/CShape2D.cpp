#include "CShape2D.h"
#include "CCamera.h"

CShape2D::CShape2D()
{
	_pPoints = NULL;
	_pNormals = NULL;
	_pTex = NULL;
	_bProjUpdated = false;
	_bViewUpdated = false;
	_bTRSUpdated = false;
	_bColorUpdated = false;
}

CShape2D::~CShape2D()
{
	if( _pPoints  != NULL ) delete [] _pPoints;  
	if( _pNormals != NULL ) delete	[] _pNormals;
	if( _pTex != NULL ) delete	_pTex;

	if( _pVXshader != NULL ) delete [] _pVXshader;
	if( _pFSshader != NULL ) delete [] _pFSshader;
}

void CShape2D::createBufferObject()
{
    glGenVertexArrays( 1, &_uiVao );
    glBindVertexArray( _uiVao );

    // Create and initialize a buffer object
    glGenBuffers( 1, &_uiBuffer );
    glBindBuffer( GL_ARRAY_BUFFER, _uiBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(vec4)*_iNumVtx + sizeof(vec3)*_iNumVtx, NULL, GL_STATIC_DRAW );

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vec4)*_iNumVtx, _pPoints ); 
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vec4)*_iNumVtx, sizeof(vec3)*_iNumVtx, _pNormals );

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CShape2D::setShader(mat4 &mxModelView, mat4 &mxProjection, GLuint uiShaderHandle)
{
	glBindVertexArray(_uiVao);
	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);

    // Load shaders and use the resulting shader program
	if( uiShaderHandle == MAX_UNSIGNED_INT) _uiProgram = InitShader( _pVXshader, _pFSshader );
	else _uiProgram = uiShaderHandle;

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( _uiProgram, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    GLuint vNormal = glGetAttribLocation( _uiProgram, "vNormal" ); 
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*_iNumVtx) );

	_uiModelView = glGetUniformLocation( _uiProgram, "ModelView" );
	_mxMVFinal = _mxView = mxModelView;
	glUniformMatrix4fv( _uiModelView, 1, GL_TRUE, _mxView );

    _uiProjection = glGetUniformLocation( _uiProgram, "Projection" );
	_mxProjection = mxProjection;
	glUniformMatrix4fv( _uiProjection, 1, GL_TRUE, _mxProjection );

    _uiColor = glGetUniformLocation( _uiProgram, "vColor" );
	glUniform4fv(_uiColor, 1, _fColor ); 

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindVertexArray(0);
}

void CShape2D::setShader(GLuint uiShaderHandle)
{
	glBindVertexArray(_uiVao);
	glBindBuffer(GL_ARRAY_BUFFER, _uiBuffer);

	// Load shaders and use the resulting shader program
	if (uiShaderHandle == MAX_UNSIGNED_INT) _uiProgram = InitShader(_pVXshader, _pFSshader);
	else _uiProgram = uiShaderHandle;

	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(_uiProgram, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(_uiProgram, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(vec4)*_iNumVtx));

	_uiModelView = glGetUniformLocation(_uiProgram, "ModelView");
	//	_mxMVFinal , _mxView �ŧi�ɴN�O���x�}
	glUniformMatrix4fv(_uiModelView, 1, GL_TRUE, _mxView);

	_uiProjection = glGetUniformLocation(_uiProgram, "Projection");
	// _mxProjection �ŧi�ɴN�O���x�}
	glUniformMatrix4fv(_uiProjection, 1, GL_TRUE, _mxProjection);

	_uiColor = glGetUniformLocation(_uiProgram, "vColor");
	glUniform4fv(_uiColor, 1, _fColor);

	glBindBuffer(GL_ARRAY_BUFFER, 0);		// �����j�w�� VBO
	glBindVertexArray(0); // �����j�w�� VAO
}

void CShape2D::drawingSetShader()
{
	glUseProgram( _uiProgram );
	glBindVertexArray( _uiVao );

	if (_bViewUpdated || _bTRSUpdated) { // Model View �������x�}���e�����
		_mxMVFinal = _mxView * _mxTRS;
		_bViewUpdated = _bTRSUpdated = false;
	}

	//update shader -- �p�G��~���|��s����A�᭱������i��ϥΨ�Ĥ@��shader
	glUniformMatrix4fv( _uiModelView, 1, GL_TRUE, _mxMVFinal );//important command

	if (_bProjUpdated ) {
		glUniformMatrix4fv( _uiProjection, 1, GL_TRUE, _mxProjection );
		_bProjUpdated = false;
	}
	if (_bColorUpdated ) {
		glUniform4fv(_uiColor, 1, _fColor);
		_bColorUpdated = false;
	}
}

// ���B�w�]�ϥΫe�@�Ӵyø����ҨϥΪ� Shader
void CShape2D::drawingWithoutSetShader()
{
	glBindVertexArray( _uiVao );
	if( _bViewUpdated || _bTRSUpdated ) { // Model View �������x�}���e�����
		_mxMVFinal = _mxView * _mxTRS;
		_bViewUpdated = _bTRSUpdated =  false;
	}
	glUniformMatrix4fv( _uiModelView, 1, GL_TRUE, _mxMVFinal );
	if (_bProjUpdated) {
		glUniformMatrix4fv( _uiProjection, 1, GL_TRUE, _mxProjection );
		_bProjUpdated = false;
	}
	glUniform4fv(_uiColor, 1, _fColor ); 
}

void CShape2D::setShaderName(const char vxShader[], const char fsShader[])
{
	int len;
	len = strlen(vxShader);
	_pVXshader = new char[len+1];
	memcpy(_pVXshader, vxShader, len+1);

	len = strlen(fsShader);
	_pFSshader = new char[len+1];
	memcpy(_pFSshader, fsShader, len+1);
}

void CShape2D::setViewMatrix(mat4 &mat)
{
	_mxView = mat;
	_bViewUpdated = true;
}

void CShape2D::setProjectionMatrix(mat4 &mat)
{
	_mxProjection = mat;
	_bProjUpdated = true;
}
void CShape2D::setTRSMatrix(mat4 &mat)
{
	_mxTRS = mat;
	_bTRSUpdated = true;
}

void CShape2D::setColor(vec4 vColor)
{
	_fColor[0] = vColor.x;
	_fColor[1] = vColor.y;
	_fColor[2] = vColor.z;
	_fColor[3] = vColor.w;
	_bColorUpdated = true;
}