# Read Data ---------------------------------------------------------------
library(ggplot2)
library(lda)
scratch <- 'C:/Users/Lichao/scratch/svm/'
traindn <- 'svmtrain'
testdn <- 'svmdata'


ns = c('car','face','motorbike','airplane')


disthist <- function(prefix, suffix, m, ns, mid ) {
  dats <<- list()
  docs <<- list()
  annot <<-list()
  for(n in ns){
    temp <-read.table(paste(prefix,mid,n,suffix,sep='_'))
    dats <<- rbind(dats,temp)
    
    annot<<- c(annot,rep(n==m, dim(temp)[1]))
  }
  annot[annot==T]<<- 1
  annot[annot==F]<<- -1
  docs <<- apply(dats,1,function(vv){paste(sapply(1:length(vv),function(i){paste(rep(as.character(i),vv[i]),collapse = " ")}),collapse = " " )})
}
suffix <- 'res_64.txt'
res<<-list()
for(m in ns){
  prefix <- paste(scratch,traindn,m,sep='/')
  disthist(prefix, suffix,m,ns,'train')
  corpus<-lexicalize(docs)
  p <- sample(c(-1, 1), 4, replace=TRUE)
  mo = slda.em(corpus$documents,corpus$vocab,K=4,annotations=unlist(annot),variance=0.25,num.e.iterations=10,num.m.iterations=4,alpha=1.0, eta=0.1,params=p)
  testprefix <-paste(scratch,testdn,m,sep='/')
  disthist(testprefix, suffix,m,ns,'test')
  testcorpus <-lexicalize(docs,vocab = corpus$vocab)
  res[[m]]=slda.predict(testcorpus,mo$topics,mo$model,alpha = 1.0,eta=0.1)
}


