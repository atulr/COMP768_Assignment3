#include "sampleGUI.h"

/////////////////////////////////////////////////////////////////
//// Main opengl stuff

// where the drawing stuff should go
static int count = 0;
bool enableIntegration = false;
void myGlutDisplay(	void )
{
    if (texCapture == 1)
        captureSceneToTexture();
    
    
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);   // clears the screen
    
    glLoadIdentity(); // reset the modelview matrix to the default state
    gluLookAt(eye[0], eye[1], eye[2], lookat[0], lookat[1], lookat[2], 0, 1, 0);    // place the camera where we want
        
    // draws the scene
    drawScene();


    
    glFlush();
	glutSwapBuffers();

}

void updateNDot() {
    
    myVec3 temp;
    
    temp = xDot.sub(x.scalarMultiply(x.dot(xDot)/x.dot(x)));
    
    NDot.vec[0] = temp.vec[0]/x.mod();
    NDot.vec[1] = temp.vec[1]/x.mod();
    NDot.vec[2] = temp.vec[2]/x.mod();
    
}

void updateN() {
    
    N.vec[0] = x.vec[0]/x.mod();
    N.vec[1] = x.vec[1]/x.mod();
    N.vec[2] = x.vec[2]/x.mod();
   
}

void updateLambda() {
    lambda = -mass * (NDot.dot(xDot)/N.dot(N)) - (N.dot(f)/(N.dot(N)));
}

void updateLambdaFeedback() {
    float C, CDot, nDotxDot, nDotN, nDotF;
    C = x.mod() - r;
    C = alpha * C;
    CDot = N.dot(xDot) * beta;
    nDotxDot = NDot.dot(xDot);
    nDotN = N.dot(N);
    nDotF = N.dot(f.add(fExt));
    
    lambda = -(mass/nDotN) * (C + CDot + nDotxDot) - nDotF/nDotN;
}

void euler() {

    xDot = xDot.add( f.add(N.scalarMultiply(lambda)).scalarMultiply(dt/mass) );

    x = x.add( xDot.scalarMultiply(dt) );

    updateN();
    updateNDot();
    updateLambda();
}

void eulerFeedback() {
    xDot = xDot.add( f.add(fExt).add(N.scalarMultiply(lambda)).scalarMultiply(dt/mass) );
    
    x = x.add( xDot.scalarMultiply(dt) );
    
    updateN();
    updateNDot();
    updateLambdaFeedback();

    
}

void rK4() {
    float k1, k2, k3, k4;
    
//  update for x[0]
    
    k1 = (vel[0] * dt);
    k2 = (vel[0] + k1/2) * dt;
    k3 = (vel[0] + k2/2) * dt;
    k4 = (vel[0] + k3) * dt;
    
    vec[0] = vec[0] + k1/6 + k2/3 + k3/3 + k4/6;
    
    //  update for x[1]
    
    k1 = (vel[1] * dt);
    k2 = (vel[1] + k1/2) * dt;
    k3 = (vel[1] + k2/2) * dt;
    k4 = (vel[1] + k3) * dt;
    
    vec[1] = vec[1] + k1/6 + k2/3 + k3/3 + k4/6;

    //  update for x[2]
    
    k1 = (vel[2] * dt);
    k2 = (vel[2] + k1/2) * dt;
    k3 = (vel[2] + k2/2) * dt;
    k4 = (vel[2] + k3) * dt;
    
    vec[2] = vec[2] + k1/6 + k2/3 + k3/3 + k4/6;
    
        
//  update for v[0]
    
    k1 = (- dragCoeff * vel[0]/mass) * dt;
    k2 = (- dragCoeff * ((vel[0] + 0.5f * k1)/mass)) * dt;
    k3 = (- dragCoeff * ((vel[0] + 0.5f * k2)/mass)) * dt;
    k4 = (- dragCoeff * ((vel[0] + k3)/mass)) * dt;
    
    vel[0] = vel[0] + k1/6 + k2/3 + k3/3 + k4/6;

// update for v[1]

    k1 = (- g - dragCoeff * vel[1]/mass) * dt;
    k2 = (- g - dragCoeff * ((vel[1] + 0.5f * k1)/mass)) * dt;
    k3 = (-g - dragCoeff * ((vel[1] + 0.5f * k2)/mass)) * dt;
    k4 = (-g - dragCoeff * ((vel[1] + k3)/mass)) * dt;
    
    vel[1] = vel[1] + k1/6 + k2/3 + k3/3 + k4/6;

// update for v[2]

    k1 = (- dragCoeff * vel[2]/mass) * dt;
    k2 = (- dragCoeff * ((vel[2] + 0.5f * k1)/mass)) * dt;
    k3 = (- dragCoeff * ((vel[2] + 0.5f * k2)/mass)) * dt;
    k4 = (- dragCoeff * ((vel[2] + k3)/mass)) * dt;
    
    vel[2] = vel[2] + k1/6 + k2/3 + k3/3 + k4/6;
    
}


void normalizeVec() {
    float mag = sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
    
    vec[0] = vec[0]/mag;
    vec[1] = vec[1]/mag;
    vec[2] = vec[2]/mag;
}

void scaleVec(float scale) {
    vec[0] = scale * vec[0];
    vec[1] = scale * vec[1];
    vec[2] = scale * vec[2];
}

// drawing elements of the scene
void drawScene(){
    //
	// draw some stuff
	//
//	if (live_draw_floor)
//	{
//        glEnable(GL_COLOR_MATERIAL);
//        
//        glPopAttrib();
//        
//        
//		glBegin(GL_TRIANGLE_FAN);
//            glColor3f(0.4f, 0.4f, 0.4f);    // note: color is state and only needs to be set once
//            glVertex3f(-100, 0, -100);
//            glVertex3f( 100, 0, -100);
//            glVertex3f( 100, 0,  100);
//            glVertex3f(-100, 0,  100);
//		glEnd();
//        
//                
//        checkGLError("after wall");
//	}
    
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(1000, 0, 0);
    glEnd();
    
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
        
        glVertex3f(0, 0, 0);
        glVertex3f(0, 1000, 0);
    glEnd();
    
    glColor3f(0.0, 0.0, 1.0);
    
    glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, 1000);
    glEnd();
    
    //update all UI variables here...
    
    dt = dtSpinner; mass = massSpinner; alpha = alphaSpinner;
    beta = betaSpinner; 
    
	if (live_draw_object)
	{
		glColor3f(0, 1, 1);
		
		glPushMatrix();
		glTranslatef(live_object_xz_trans[0], live_object_y_trans, -live_object_xz_trans[1]);
//        glTranslatef(0.0, 0.0, 0.0);
        
        
//        normalizeVec();
        
//        if(count == 0) {
//            vec[0] =  cos( phi * PI/180 ) * sin( theta * PI/180 );
//            vec[1] =  -sin( phi * PI/180 );
//            vec[2] =  cos( phi * PI/180 ) * cos( theta * PI/180 );
//
//            
//            vel[0] = vec[0] * powderForce * powderMass/mass;
//            vel[1] = vec[1] * powderForce * powderMass/mass;
//            vel[2] = vec[2] * powderForce * powderMass/mass;
//            scaleVec(5.f);
//            count = 1;
//        }
        
        
         
        
        
//        glRotatef(theta, 0.0, 1.0, 0.0);  
//        glRotatef(phi, 1.0, 0.0, 0.0);
        
        
//		glMultMatrixf(live_object_rotation);
        
//        }
        
		switch(live_object_type)
		{
            case 0:
                glColor3f(1.0, 1.0, 1.0);
                glutSolidTorus(0.1f, r, 300, 300);
//                gluCylinder(gluNewQuadric(), 0.5, 0.5, 5.0, 100, 100);
//                glutSolidCube(2);
                break;
            case 1:
                glutSolidSphere(2, 30, 30);
                break;
            case 2:
                glutSolidTorus(0.5, 2, 30, 30);
                break;
		}
        
		glPopMatrix();
        
        if(enableIntegration && live_simulation_type == 0)
            euler();

        if(enableIntegration && live_simulation_type == 1)
            eulerFeedback();
        
        glPushMatrix();
        glColor3f(1.0, 1.0, 0.0);
        glTranslatef(x.vec[0], x.vec[1], x.vec[2]);
        
        glutSolidSphere(.5, 30, 30);
        
        glPopMatrix();
	}
}

void captureSceneToTexture(){
    glViewport(0,0, texSizeX,texSizeY);   // set viewport to texture size
    
    
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    checkGLFrameBufferError(GL_FRAMEBUFFER, "After FBO binding");
        
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);   // clears the screen
        
    glLoadIdentity(); // reset the modelview matrix to the default state
    gluLookAt(eye[0], eye[1], eye[2], lookat[0], lookat[1], lookat[2], 0, 1, 0);    // place the camera where we want
        
    // draws the scene
    drawScene();
        
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    checkGLFrameBufferError(GL_FRAMEBUFFER, "After releasing buffer");
        
    //should be used only for debugging; writing texture to disk is slow!!!!!!!!!!
    //put in a path you want your texture to be in
    writeTextureToPPM("/Users/pascalgrosset/Desktop/textureSaved", textureCapture, texSizeX, texSizeX, GL_RGBA, GL_UNSIGNED_BYTE);
    
    
    texCapture = 0;     // toggle screen capture off
        
    myGlutReshape(0,0); // force reset of the viewport
}


// init openGL states
void initGL(){
    // display some opengl info
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Shading language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl << std::endl;
    
    // load the textures you need
    LoadGLTextures();
    
    // sets up a frame buffer object
    if (setUpFrameBufferObj() == 1 )
        std::cout << "Framebuffer creation failed!!!" << std::endl << std::endl;

    // color to clear the screen to
    glClearColor(0, 0, 0, 0);
    
    // set some states
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
}

   

// settiug up a frambeuffer object for capture to texture
// directly taken from: http://www.songho.ca/opengl/gl_fbo.html
int setUpFrameBufferObj(){
    // create the texture to capture the texture to
    glGenTextures(1, &textureCapture);
    glBindTexture(GL_TEXTURE_2D, textureCapture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, texSizeX, texSizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    
    // create a renderbuffer object to store depth info
    GLuint rboId;
    glGenRenderbuffers(1, &rboId);
    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, texSizeX, texSizeY);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    
    // create a framebuffer object
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    
    // attach the texture to FBO color attachment point
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureCapture, 0);
    
    // attach the renderbuffer to depth attachment point
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);
    
    // check FBO status
    GLenum status = checkGLFrameBufferError(GL_FRAMEBUFFER, "After creating buffer");
    if (status != GL_FRAMEBUFFER_COMPLETE)
        return 1;
    
    // switch back to window-system-provided framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return 0;
}


/////////////////////////////////////////////////////////////////
///// Input stuff

// catch mouse up/down events
void myGlutMouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
		cur_button = button;
	else
	{
		if (button == cur_button) {
            fExt.vec[0] = 0.f;
            fExt.vec[1] = 0.f;
            fExt.vec[2] = 0.f;
			cur_button = -1;
        }
	}

	last_x = x;
	last_y = y;
}


void reset() {
    x.reset(5.f, 0.f, 0.f);
    xDot.reset(0.f, 0.f, 0.f);
    N.reset(0.f, 0.f, 0.f);
    NDot.reset(0.f, 0.f, 0.f);
    
}

// catch mouse move events
void myGlutMotion(int xC, int yC)
{
	// the change in mouse position
	int dx = xC-last_x;
	int dy = yC-last_y;
    
    GLint viewport[4];
    GLdouble modelview[16],projection[16];
    GLfloat wx=xC,wy,wz;

	float scale, len, theta;
	float neye[3], neye2[3];
	float f[3], r[3], u[3];
    float dist, scaling;

	switch(cur_button)
	{
	case GLUT_LEFT_BUTTON:
        // this piece of code is taken from this link: http://stackoverflow.com/questions/12832102/incorrect-3d-coordinates-from-2d-mouse-click-in-opengl
        
        glGetIntegerv(GL_VIEWPORT,viewport);
        yC=viewport[3]-yC;
        wy=yC;
        glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
        glGetDoublev(GL_PROJECTION_MATRIX,projection);
        glReadPixels(xC,yC,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
        gluProject(x.vec[0], x.vec[1], x.vec[2], modelview, projection, viewport, &ox, &oy, &oz);
//        gluUnProject(wx,wy,wz,modelview,projection,viewport,&ox,&oy,&oz);
        
        printf(" %d %d \n", xC, yC);
        printf(" %f %f \n", ox, oy);
        
        fExt.vec[0] = x.vec[0] - ox;
        fExt.vec[1] = x.vec[1] - oy;
        fExt.vec[2] = x.vec[2] - oz;
        dist = fExt.mod();
        scaling = dist * extForceScaling;
        fExt.normalize();
        fExt = fExt.scalarMultiply(forceScalingSpinner);
        
//		// translate
//		f[0] = lookat[0] - eye[0];
//		f[1] = lookat[1] - eye[1];
//		f[2] = lookat[2] - eye[2];
//		u[0] = 0;
//		u[1] = 1;
//		u[2] = 0;
//
//		// scale the change by how far away we are
//		scale = sqrt(length(f)) * 0.007;
//
//		crossproduct(f, u, r);
//		crossproduct(r, f, u);
//		normalize(r);
//		normalize(u);
//
//		eye[0] += -r[0]*dx*scale + u[0]*dy*scale;
//		eye[1] += -r[1]*dx*scale + u[1]*dy*scale;
//		eye[2] += -r[2]*dx*scale + u[2]*dy*scale;
//
//		lookat[0] += -r[0]*dx*scale + u[0]*dy*scale;
//		lookat[1] += -r[1]*dx*scale + u[1]*dy*scale;
//		lookat[2] += -r[2]*dx*scale + u[2]*dy*scale;

		break;

	case GLUT_MIDDLE_BUTTON:
		// zoom
		f[0] = lookat[0] - eye[0];
		f[1] = lookat[1] - eye[1];
		f[2] = lookat[2] - eye[2];

		len = length(f);
		normalize(f);

		// scale the change by how far away we are
		len -= sqrt(len)*dx*0.03;

		eye[0] = lookat[0] - len*f[0];
		eye[1] = lookat[1] - len*f[1];
		eye[2] = lookat[2] - len*f[2];

		// make sure the eye and lookat points are sufficiently far away
		// push the lookat point forward if it is too close
		if (len < 1)
		{
			printf("lookat move: %f\n", len);
			lookat[0] = eye[0] + f[0];
			lookat[1] = eye[1] + f[1];
			lookat[2] = eye[2] + f[2];
		}

		break;

	case GLUT_RIGHT_BUTTON:
		// rotate

		neye[0] = eye[0] - lookat[0];
		neye[1] = eye[1] - lookat[1];
		neye[2] = eye[2] - lookat[2];

		// first rotate in the x/z plane
		theta = -dx * 0.007;
		neye2[0] = (float)cos(theta)*neye[0] + (float)sin(theta)*neye[2];
		neye2[1] = neye[1];
		neye2[2] =-(float)sin(theta)*neye[0] + (float)cos(theta)*neye[2];


		// now rotate vertically
		theta = -dy * 0.007;

		f[0] = -neye2[0];
		f[1] = -neye2[1];
		f[2] = -neye2[2];
		u[0] = 0;
		u[1] = 1;
		u[2] = 0;
		crossproduct(f, u, r);
		crossproduct(r, f, u);
		len = length(f);
		normalize(f);
		normalize(u);

		neye[0] = len * ((float)cos(theta)*f[0] + (float)sin(theta)*u[0]);
		neye[1] = len * ((float)cos(theta)*f[1] + (float)sin(theta)*u[1]);
		neye[2] = len * ((float)cos(theta)*f[2] + (float)sin(theta)*u[2]);

		eye[0] = lookat[0] - neye[0];
		eye[1] = lookat[1] - neye[1];
		eye[2] = lookat[2] - neye[2];

		break;
	}


	last_x = xC;
	last_y = yC;

	glutPostRedisplay();
}

// you can put keyboard shortcuts in here
void myGlutKeyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	// quit
	case 27: 
	case 'q':
	case 'Q':
		exit(0);
		break;
	}

	glutPostRedisplay();
}



/////////////////////////////////////////////////////////////////
///// some opengl utilities stuff

// the window has changed shapes, fix ourselves up
void myGlutReshape(int	x, int y)
{
	int tx, ty, tw, th;
	GLUI_Master.get_viewport_area(&tx, &ty, &tw, &th);
	glViewport(tx, ty, tw, th);
    
    
    // projection transform
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1, 1000);
    
	// camera transform
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    

	glutPostRedisplay();
}


// make sure the main window is active
void myGlutIdle(void)
{
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);
	
	// just keep redrawing the scene over and over
	glutPostRedisplay();
}




/////////////////////////////////////////////////////////////////
///// UI stuff

// some controls generate a callback when they are changed
void glui_cb(int control)
{

	switch(control)
	{
	case CB_DEPTH_BUFFER:
		if (live_use_depth_buffer)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
		break;
	case CB_RESET_BUTTON:
            reset();
            enableIntegration = false;
		break;
            
	case CB_RESET:
		//
		// put your reset callback here
		//
		break;
    case CB_FIRE_BUTTON:
        enableIntegration = true;
        break;
            
	}

	glutPostRedisplay();
}

void glui_button(int control){
	switch(control){
		case TEX_CAPTURE:
			texCapture = 1-texCapture;
			break;
            
        default:
            break;
    }
}


// the UI code
void initGLUI(){
    //
	// create the interface subwindow and add widgets
	//
	glui = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_LEFT);
    
	// initialize live variables
	live_object_type = 0;
	live_object_xz_trans[0] = 0;
	live_object_xz_trans[1] = 0;
	live_object_y_trans = 0;
	live_draw_floor = 1;
	live_draw_object = 1;
	live_use_depth_buffer = 1;
    
	// quit button
	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);
    
	// empty space
	glui->add_statictext("");
    
	// the object rollout
	object_rollout = glui->add_rollout("Simulation");
    
	// the radio buttons
    simulationType = glui->add_radiogroup_to_panel(object_rollout, &live_simulation_type);
    
//	object_type_radio = glui->add_radiogroup_to_panel(object_rollout, &live_object_type);
    glui->add_radiobutton_to_group(simulationType, "No Feedback");
    glui->add_radiobutton_to_group(simulationType, "Feedback");
//	glui->add_radiobutton_to_group(object_type_radio, "cube");
//	glui->add_radiobutton_to_group(object_type_radio, "sphere");
//	glui->add_radiobutton_to_group(object_type_radio, "torus");
    
    
    
   
	// rotation and translation controls
	// we need an extra panel to keep things grouped properly
	GLUI_Panel *transform_panel = glui->add_panel_to_panel(object_rollout, "", GLUI_PANEL_NONE);
	object_rotation = glui->add_rotation_to_panel(transform_panel, "Rotation", live_object_rotation);
	object_rotation->reset();
    
	glui->add_column_to_panel(transform_panel, false);
	object_xz_trans = glui->add_translation_to_panel(transform_panel, "Translate XZ",
													 GLUI_TRANSLATION_XY, live_object_xz_trans);
    
	glui->add_column_to_panel(transform_panel, false);
	object_y_trans =  glui->add_translation_to_panel(transform_panel, "Translate Y",
													 GLUI_TRANSLATION_Y, &live_object_y_trans);
    
	object_xz_trans->scale_factor = 0.1f;
	object_y_trans->scale_factor = 0.1f;
    
	glui->add_button_to_panel(object_rollout, "Reset Object Transform", CB_RESET, glui_cb);
    
	// empty space
	glui->add_statictext("");
    
	// the walk control
	anim_rollout = glui->add_rollout("Animation");
    
	reset_button = glui->add_button_to_panel(anim_rollout, "Reset", CB_RESET_BUTTON, glui_cb);

	fire_button = glui->add_button_to_panel(anim_rollout, "Fire", CB_FIRE_BUTTON, glui_cb);
    
	GLUI_Spinner *spin_s =
    
    glui->add_spinner_to_panel(anim_rollout, "dt", GLUI_SPINNER_FLOAT, &dtSpinner);
    
	spin_s->set_float_limits(0.01, 1.0);
    
    GLUI_Spinner *spin_mass = 

    glui->add_spinner_to_panel(anim_rollout, "Mass", GLUI_SPINNER_FLOAT, &massSpinner);
    
    spin_mass->set_float_limits(0.1, 10.0);
    
    GLUI_Spinner *spin_alpha =
    
    glui->add_spinner_to_panel(anim_rollout, "Alpha", GLUI_SPINNER_FLOAT, &alphaSpinner);
    
    spin_alpha->set_float_limits(0.0, 1.0);

    GLUI_Spinner *spin_beta =
    
    glui->add_spinner_to_panel(anim_rollout, "Beta", GLUI_SPINNER_FLOAT, &betaSpinner);
    
    spin_beta->set_float_limits(0.0, 1.0);
    
    GLUI_Spinner *spin_forcescaling =
    
    glui->add_spinner_to_panel(anim_rollout, "Force Scaling", GLUI_SPINNER_FLOAT, &forceScalingSpinner);
    
    spin_forcescaling->set_float_limits(1.0, 40.0);
    

	// our checkbox options for deciding what to draw
	glui->add_checkbox("Draw Floor", &live_draw_floor);
	glui->add_checkbox("Draw Object", &live_draw_object);
    
	// empty space
	glui->add_statictext("");
    
	glui->add_checkbox("Use Depth Buffer", &live_use_depth_buffer, CB_DEPTH_BUFFER, glui_cb);
    
    
    // empty space
	glui->add_statictext("");
    
    glui->add_button("Capture to texture", TEX_CAPTURE, glui_button);
    
	glui->set_main_gfx_window(main_window);
}


/////////////////////////////////////////////////////////////////
///// entry point

int main(int argc,	char* argv[])
{
    glutInit(&argc, argv);
    
	//
	// create the glut window
	//
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(1000, 600);
	glutInitWindowPosition(100,100);
    
    rotMatrixX = (float**) malloc(3 * sizeof(float*));
    rotMatrixY = (float**) malloc(3 * sizeof(float*));
    vec = (float*) malloc(3 * sizeof(float));
    
    x.vec[0] = 5.f;
    x.vec[1] = 0.f;
    x.vec[2] = 0.f;
    
    xDot.vec[0] = 0.f;
    xDot.vec[1] = 0.f;
    xDot.vec[2] = 0.f;
    
    updateN();
    updateNDot();
    
    
	main_window = glutCreateWindow("Sample Interface");
	
	//
	// set callbacks
	//
	glutDisplayFunc(myGlutDisplay);
	GLUI_Master.set_glutReshapeFunc(myGlutReshape);
	GLUI_Master.set_glutIdleFunc(myGlutIdle);
	GLUI_Master.set_glutKeyboardFunc(myGlutKeyboard);
	GLUI_Master.set_glutMouseFunc(myGlutMouse);
	glutMotionFunc(myGlutMotion);

    
    // initialize gl
    initGL();
    
    // init the interface
	initGLUI();

    
	// initialize the camera
	eye[0] = 0; 	eye[1] = 4;     eye[2] = 10;
	lookat[0] = 0;	lookat[1] = 0;	lookat[2] = 0;

    
	
	// give control over to glut
	glutMainLoop();

	return 0;
}
