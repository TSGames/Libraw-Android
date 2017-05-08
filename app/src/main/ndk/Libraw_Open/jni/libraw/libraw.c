#include "libraw/libraw.h"
#include <jni.h>
#include <android/log.h>

libraw_data_t* librawData=NULL;
libraw_processed_image_t* image=NULL;
char* image16=NULL;
void cleanup(){
	if(librawData!=NULL){
		libraw_recycle(librawData);
		librawData=NULL;
	}
	if(image!=NULL){
		libraw_dcraw_clear_mem(image);
		image=NULL;
	}
}
JNIEXPORT void JNICALL Java_com_tssystems_Libraw_cleanup(JNIEnv * env, jobject obj){
	cleanup();
}
JNIEXPORT jint JNICALL Java_com_tssystems_Libraw_open(JNIEnv * env, jobject obj,jstring file){
	cleanup();
	const char *nativeString = (*env)->GetStringUTFChars(env, file, 0);
	__android_log_print(ANDROID_LOG_INFO,"libraw","open %s",nativeString);
	librawData=libraw_init(0);
	int result=libraw_open_file(librawData,nativeString);
	if(result==0){
		result=libraw_unpack(librawData);
	}
	(*env)->ReleaseStringUTFChars(env, file, nativeString);
	return result;	
}
JNIEXPORT jint JNICALL Java_com_tssystems_Libraw_getWidth(){
	return librawData->sizes.width;
	
}
JNIEXPORT jint JNICALL Java_com_tssystems_Libraw_getBitmapWidth(){
	return image->width;
}
JNIEXPORT jint JNICALL Java_com_tssystems_Libraw_getBitmapHeight(){
	return image->height;
}

JNIEXPORT jint JNICALL Java_com_tssystems_Libraw_getHeight(){
	return librawData->sizes.height;

}
JNIEXPORT jint JNICALL Java_com_tssystems_Libraw_getOrientation(){
	return librawData->sizes.flip;
}
JNIEXPORT jbyteArray JNICALL Java_com_tssystems_Libraw_getThumbnail(JNIEnv * env, jobject obj, jstring file){
	cleanup();
	const char *nativeString = (*env)->GetStringUTFChars(env, file, 0);
	librawData=libraw_init(0);
	int result=libraw_open_file(librawData,nativeString);
	if(result==0)
		result=libraw_unpack_thumb(librawData);
	
	(*env)->ReleaseStringUTFChars(env, file, nativeString);
	if(result==0){
		jbyteArray jbyteArray = (*env)->NewByteArray(env, librawData->thumbnail.tlength);
		if(jbyteArray==NULL)
			return NULL;
		(*env)->SetByteArrayRegion(env, jbyteArray, 0, librawData->thumbnail.tlength,(jbyte*)librawData->thumbnail.thumb);
		return jbyteArray;
	}
	return NULL;
}
JNIEXPORT jfloatArray JNICALL Java_com_tssystems_Libraw_getDaylightMultiplier(JNIEnv * env, jobject obj){
	float* mul = librawData->color.pre_mul;
	jfloatArray jfloatArray = (*env)->NewFloatArray(env, 3);
	(*env)->SetFloatArrayRegion(env, jfloatArray, 0, 3, mul);
	return jfloatArray;
}
JNIEXPORT jfloatArray JNICALL Java_com_tssystems_Libraw_getWhitebalanceMultiplier(JNIEnv * env, jobject obj){
	float* mul = librawData->color.cam_mul;
	jfloatArray jfloatArray = (*env)->NewFloatArray(env, 3);
	(*env)->SetFloatArrayRegion(env, jfloatArray, 0, 3, mul);
	return jfloatArray;
}
void CLASS pseudoinverse (float (*in)[3], float (*out)[3], int size)
{
  float work[3][6], num;
  int i, j, k;

  for (i=0; i < 3; i++) {
    for (j=0; j < 6; j++)
      work[i][j] = j == i+3;
    for (j=0; j < 3; j++)
      for (k=0; k < size; k++)
	work[i][j] += in[k][i] * in[k][j];
  }
  for (i=0; i < 3; i++) {
    num = work[i][i];
    for (j=0; j < 6; j++)
      work[i][j] /= num;
    for (k=0; k < 3; k++) {
      if (k==i) continue;
      num = work[k][i];
      for (j=0; j < 6; j++)
	work[k][j] -= work[i][j] * num;
    }
  }
  for (i=0; i < size; i++)
    for (j=0; j < 3; j++)
      for (out[i][j]=k=0; k < 3; k++)
	out[i][j] += work[j][k+3] * in[i][k];
}

JNIEXPORT jfloatArray JNICALL Java_com_tssystems_Libraw_getCamRgb(JNIEnv * env, jobject obj){
	if(librawData==NULL)
		return NULL;
	
	float inverse[4][3];
	float cam_rgb[4][3];
	
	int j,i;
	for (i=0; i < 4; i++)
		for (j=0; j < 3; j++)
			cam_rgb[i][j] = librawData->color.rgb_cam[j][i];
	
	pseudoinverse(cam_rgb,inverse,3);
	/*
	int j,i;
	for (i=0; i < 3; i++)
		for (j=0; j < 4; j++)
      rgb_cam[i][j] = inverse[j][i];
	*/
	jfloatArray jfloatArray = (*env)->NewFloatArray(env, 12);
	(*env)->SetFloatArrayRegion(env, jfloatArray, 0, 12,(jfloat*)inverse);
	return jfloatArray;
}
JNIEXPORT jfloatArray JNICALL Java_com_tssystems_Libraw_getRgbCam(JNIEnv * env, jobject obj){
	float* mul = (float*)librawData->color.rgb_cam;
	jfloatArray jfloatArray = (*env)->NewFloatArray(env, 12);
	(*env)->SetFloatArrayRegion(env, jfloatArray, 0, 12, mul);
	return jfloatArray;
}
JNIEXPORT jfloatArray JNICALL Java_com_tssystems_Libraw_getCamMatrix(JNIEnv * env, jobject obj){
	float* mul = (float*)librawData->color.cmatrix;
	jfloatArray jfloatArray = (*env)->NewFloatArray(env, 12);
	(*env)->SetFloatArrayRegion(env, jfloatArray, 0, 12, mul);
	return jfloatArray;
}
JNIEXPORT jint JNICALL Java_com_tssystems_Libraw_getColors(JNIEnv * env, jobject obj){
	return librawData->rawdata.iparams.colors;
}
JNIEXPORT void JNICALL Java_com_tssystems_Libraw_setUseCameraMatrix(JNIEnv * env, jobject obj,jint use_camera_matrix){
	librawData->params.use_camera_matrix=use_camera_matrix;
}
JNIEXPORT void JNICALL Java_com_tssystems_Libraw_setQuality(JNIEnv * env, jobject obj,jint quality){
	librawData->params.user_qual=quality;
}
JNIEXPORT void JNICALL Java_com_tssystems_Libraw_setAutoBrightness(JNIEnv * env, jobject obj,jboolean autoBrightness){
	librawData->params.no_auto_bright=!autoBrightness;
}
JNIEXPORT void JNICALL Java_com_tssystems_Libraw_setAutoWhitebalance(JNIEnv * env, jobject obj,jboolean autoWhitebalance){
	librawData->params.use_camera_wb=autoWhitebalance;
}
JNIEXPORT void JNICALL Java_com_tssystems_Libraw_setOutputColorSpace(JNIEnv * env, jobject obj,jint space){
	librawData->params.output_color=space;
}
JNIEXPORT void JNICALL Java_com_tssystems_Libraw_setHighlightMode(JNIEnv * env, jobject obj,jint highlight){
	librawData->params.highlight=highlight;
}
JNIEXPORT void JNICALL Java_com_tssystems_Libraw_setOutputBps(JNIEnv * env, jobject obj,jint output_bps){
	librawData->params.output_bps=output_bps;
}
JNIEXPORT void JNICALL Java_com_tssystems_Libraw_setHalfSize(JNIEnv * env, jobject obj,jboolean half_size){
	librawData->params.half_size=half_size;
}

JNIEXPORT void JNICALL Java_com_tssystems_Libraw_setUserMul(JNIEnv * env, jobject obj,jfloat r,jfloat g1,jfloat b,jfloat g2){
	librawData->params.user_mul[0]=r;
	librawData->params.user_mul[1]=g1;
	librawData->params.user_mul[2]=b;
	librawData->params.user_mul[3]=g2;
}
JNIEXPORT void JNICALL Java_com_tssystems_Libraw_setGamma(JNIEnv * env, jobject obj,jdouble g1,jdouble g2){
	librawData->params.gamm[0]=g1;
	librawData->params.gamm[1]=g2;
}
libraw_processed_image_t* decode(int* error){
	int dcraw=libraw_dcraw_process(librawData);
	__android_log_print(ANDROID_LOG_INFO,"libraw","result dcraw %d",dcraw);
	return libraw_dcraw_make_mem_image(librawData,error);
}
JNIEXPORT jstring JNICALL Java_com_tssystems_Libraw_getCameraList(JNIEnv * env, jobject obj){
	jstring result;
	char message[1024*1024];
	strcpy(message,"");
	const char** list=libraw_cameraList();
	for(int i=0;i<libraw_cameraCount();i++){
		strcat(message,list[i]);
		strcat(message,"\n");
	}
	result = (*env)->NewStringUTF(env,message); 
 return result;       
}
JNIEXPORT jintArray JNICALL Java_com_tssystems_Libraw_getPixels8(JNIEnv * env, jobject obj){
	int error;
	image=decode(&error);
	if(image!=NULL){
		int* image8 = (int*)malloc(sizeof(int)*image->width*image->height);
		if(image8==NULL){
			__android_log_print(ANDROID_LOG_INFO,"libraw","getPixels8 oom");
			return NULL;
		}
		__android_log_print(ANDROID_LOG_INFO,"libraw","getPixels8 image colors %d",image->colors);
		for(int y=0;y<image->height;y++){
			for(int x=0;x<image->width;x++){
				int pos=(x+y*image->width)*3;
				image8[x+y*image->width]=	0xff000000 | (image->data[pos]<<16) | (image->data[pos+1]<<8) | (image->data[pos+2]);
			}
		}			
		jintArray jintArray = (*env)->NewIntArray(env, image->width*image->height);
		(*env)->SetIntArrayRegion(env, jintArray, 0, image->width*image->height, image8);
		free(image8);
		return jintArray;
	}	
	__android_log_print(ANDROID_LOG_INFO,"libraw","error getPixels8 %d",error);
	return NULL;
}
JNIEXPORT jlong JNICALL Java_com_tssystems_Libraw_getPixels16(JNIEnv * env, jobject obj){
	int error;
	image=decode(&error);
	__android_log_print(ANDROID_LOG_INFO,"libraw","decode result %d data_size %d",error,image->data_size);
	if(image!=NULL && image->data_size){
		__android_log_print(ANDROID_LOG_INFO,"libraw","image length %d",image->data_size);
		libraw_recycle(librawData);
		librawData=NULL;
		image16=malloc(image->data_size);
		if(image16==NULL){
			__android_log_print(ANDROID_LOG_INFO,"libraw","getPixels16 oom");
			return 0;
		}
		__android_log_print(ANDROID_LOG_INFO,"libraw","allocated memory");
		memcpy(image16,image->data,image->data_size);
		__android_log_print(ANDROID_LOG_INFO,"libraw","copied pointer");
		return (jlong)image16;
	}
	return 0;
}