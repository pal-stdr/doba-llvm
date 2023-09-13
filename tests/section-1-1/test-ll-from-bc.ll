; ModuleID = 'tests/section-1-1/test.bc'
source_filename = "tests/section-1-1/test.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
  ret i32 1000
}
