/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.content.Intent;
import android.database.Cursor;
import android.net.Uri;
import android.util.Log;
import android.os.Bundle;
import android.provider.MediaStore;

public class AppActivity extends Cocos2dxActivity {
	public static AppActivity instance;
	public static Object getInstance() {
		return instance;
	}
	
	public void onCreate(Bundle saveInsanceState) {
		super.onCreate(saveInsanceState);
		instance = this;
	}
	
	// java call c++ method
	private native static void setImagePath(final String str);
	
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		
		if (resultCode == RESULT_OK) {
			Uri uri = data.getData();
			Log.e("cocos2d-x debug info", uri.toString());
			
	        String[] proj = { MediaStore.Images.Media.DATA };
	        Cursor cursor = managedQuery(uri, proj, null, null, null);
	        int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
	        cursor.moveToFirst();
	        
	        Log.e("cocos2d-x debug info", cursor.getString(column_index));
	        setImagePath(cursor.getString(column_index));
		}
	}
	
	// c++ call java
	public void onSelect() {
		Log.e("cocos2d-x debug info", "onSelect");
		Intent it = new Intent();
		it.setType("image/*");
		it.setAction(Intent.ACTION_GET_CONTENT);
		startActivityForResult(it, 1);	// 目前看来第二个参数可以随便写
	}
	
	// c++ call java
	public void callNumber(String uriString) {
		Log.e("cocos2d-x debug info", "callNumber: " + uriString);
		Intent it = new Intent(Intent.ACTION_CALL, Uri.parse(uriString));
		startActivity(it);
	}
}
