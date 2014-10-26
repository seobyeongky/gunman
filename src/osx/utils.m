//
//  utils.m
//  gunman
//
//  Created by 서병기 on 2014. 10. 14..
//  Copyright (c) 2014년 ooparts. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#import "utils.h"

int MessageBox_cocoa(const char * title, const char * msg)
{
    NSAlert *alert = [[NSAlert alloc] init];
    [alert setMessageText:[NSString stringWithUTF8String:title]];
    [alert setInformativeText:[NSString stringWithUTF8String:msg]];
    [alert addButtonWithTitle:@"Ok"];
    [alert runModal];
    
    return 1;
}