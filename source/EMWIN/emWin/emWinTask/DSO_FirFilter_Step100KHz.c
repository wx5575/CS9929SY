/*
*********************************************************************************************************
*	                                  
*	ģ������ : Fir��ͨ�˲������
*	�ļ����� : DSO_FirFilter_Step100KHz.c
*	��    �� : V1.0
*	˵    �� : ����ʾ����������ʾ���˲�������80��Fir�˲����� 
*              ʹ��������Fs(������) = 6Mspsʱ����Scale������ʾ500ns/3ʱ��Fc(��ֹƵ��)��ѡ��֧�ֽ�ֹƵ��
*              Fc = 100KHz
*              Fc = 200KHz
*              Fc = 300KHz
*              Fc = 400KHz
*              Fc = 500KHz
*              Fc = 600KHz
*              Fc = 700KHz
*              Fc = 800KHz
*              Fc = 900KHz
*              Fc = 1000KHz        
*	�޸ļ�¼ :
*		�汾��    ����          ����         ˵��
*		V1.0    2015-01-05    Eric2013       �׷�
*	
*	Copyright (C), 2015-2016, ���������� www.armfly.com
*
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"


/*
*********************************************************************************************************
*	                                  �궨��
*********************************************************************************************************
*/
#define TEST_LENGTH_SAMPLES  650    /* �������� */
#define BLOCK_SIZE           650    /* ����һ��arm_fir_f32�����Ĳ�������� */
#define NUM_TAPS             81     /* �˲���ϵ������ */

/*
*********************************************************************************************************
*	                                 ����
*********************************************************************************************************
*/
float32_t FirDataInput[TEST_LENGTH_SAMPLES];        /* ������ */
float32_t FirDataOutput[TEST_LENGTH_SAMPLES];       /* �˲������� */
float32_t firStateF32[BLOCK_SIZE + NUM_TAPS - 1];   /* ״̬���棬��СnumTaps + blockSize - 1*/

/*
*********************************************************************************************************
*	                                 �˲���ϵ��
*********************************************************************************************************
*/
/* 80��FIR��ͨ�˲���ϵ�� ͨ��fadtool��ȡϵ�� Fs = 6Msps, Fc = 100KHz */
const float32_t firCoeffs32LP_100KHz[NUM_TAPS] = {
	-0.0005782532971f, -0.000563859765f,   -0.0005592969828f,  -0.0005597479176f,-0.0005588005879f,
	-0.0005485490547f, -0.0005197477876f,  -0.0004620168475f,-0.0003640938958f,-0.0002141268487f,
	2.926453079e-19f,  0.0002903142595f,   0.0006683878019f, 0.001144962618f, 0.001729581505f,
	0.002430232475f,   0.003253019415f,    0.004201866221f, 0.005278266966f, 0.006481085904f,
	0.007806419395f,   0.009247520007f,    0.01079479046f,  0.01243585069f,  0.01415566728f,
	0.01593676209f,    0.01775948144f,     0.01960232481f,  0.02144232765f,   0.0232554879f,
	0.02501723543f,    0.02670290694f,     0.02828825638f,  0.02974994481f,  0.03106603958f,
	0.03221646324f,    0.03318343312f,     0.03395184502f,  0.03450959176f,  0.03484783322f,
	0.03496118262f,    0.03484783322f,     0.03450959176f,  0.03395184502f,  0.03318343312f,
	0.03221646324f,    0.03106603958f,     0.02974994481f,  0.02828825638f,  0.02670290694f,
	0.02501723543f,    0.0232554879f,      0.02144232765f,  0.01960232481f,  0.01775948144f,
	0.01593676209f,    0.01415566728f,     0.01243585069f,  0.01079479046f, 0.009247520007f,
	0.007806419395f,   0.006481085904f,    0.005278266966f, 0.004201866221f, 0.003253019415f,
	0.002430232475f,   0.001729581505f,    0.001144962618f,0.0006683878019f,0.0002903142595f,
	2.926453079e-19f,  -0.0002141268487f,  -0.0003640938958f,-0.0004620168475f,-0.0005197477876f,
	-0.0005485490547f,-0.0005588005879f,   -0.0005597479176f,-0.0005592969828f,-0.000563859765f,
	-0.0005782532971f
};

/* 80��FIR��ͨ�˲���ϵ�� ͨ��fadtool��ȡϵ�� Fs = 6Msps, Fc = 200KHz */
const float32_t firCoeffs32LP_200KHz[NUM_TAPS] = {
  0.0005502292188f,0.0006307326839f,0.0007122113602f,0.0007916286122f,0.0008603398455f,
  0.0009040691657f,0.0009036044357f,0.0008362182998f,0.0006777559756f,0.0004052667937f,
  -5.569255587e-19f,-0.0005494627985f,-0.001244195038f,-0.002072302625f,-0.003006953746f,
  -0.004005290568f,-0.005008409731f,-0.005942528136f,-0.006721369922f,-0.007249732967f,
  -0.007428094745f,-0.007158040069f,-0.006348222494f,-0.004920508247f, -0.00281592086f,
  1.857105261e-18f, 0.003532811068f, 0.007756075822f,  0.01260984782f,  0.01800090447f,
    0.02380481735f,   0.0298698321f,  0.03602239862f,  0.04207413271f,  0.04782985523f,
    0.05309627205f,  0.05769086257f,  0.06145047769f,  0.06423915178f,  0.06595468521f,
    0.06653369218f,  0.06595468521f,  0.06423915178f,  0.06145047769f,  0.05769086257f,
    0.05309627205f,  0.04782985523f,  0.04207413271f,  0.03602239862f,   0.0298698321f,
    0.02380481735f,  0.01800090447f,  0.01260984782f, 0.007756075822f, 0.003532811068f,
  1.857105261e-18f, -0.00281592086f,-0.004920508247f,-0.006348222494f,-0.007158040069f,
  -0.007428094745f,-0.007249732967f,-0.006721369922f,-0.005942528136f,-0.005008409731f,
  -0.004005290568f,-0.003006953746f,-0.002072302625f,-0.001244195038f,-0.0005494627985f,
  -5.569255587e-19f,0.0004052667937f,0.0006777559756f,0.0008362182998f,0.0009036044357f,
  0.0009040691657f,0.0008603398455f,0.0007916286122f,0.0007122113602f,0.0006307326839f,
  0.0005502292188f
};

/* 80��FIR��ͨ�˲���ϵ�� ͨ��fadtool��ȡϵ�� Fs = 6Msps, Fc = 300KHz */
const float32_t firCoeffs32LP_300KHz[NUM_TAPS] = {
  -3.126438727e-19f,-0.0002058673272f,-0.0004228431499f,-0.0006468905485f,-0.0008642434841f,
  -0.001048665727f,-0.001161655295f,-0.001156177954f,-0.0009838859551f,-0.0006050768425f,
  8.391786605e-19f,0.0008203662583f,  0.00180617522f, 0.002865221584f, 0.003865677863f,
   0.004645895679f, 0.005031134468f, 0.004856021609f,  0.00399050815f, 0.002366269007f,
  -2.110346192e-18f,-0.002989985514f,-0.006377025973f,-0.009831513278f, -0.01293939352f,
    -0.0152332196f, -0.01623356342f, -0.01549717132f, -0.01266706176f,   -0.007519159f,
  3.381513724e-18f, 0.009749332443f,   0.0213866625f,  0.03438147902f,  0.04804687202f,
    0.06158847734f,  0.07416618615f,  0.08496309072f,  0.09325480461f,  0.09847255051f,
     0.1002533659f,  0.09847255051f,  0.09325480461f,  0.08496309072f,  0.07416618615f,
    0.06158847734f,  0.04804687202f,  0.03438147902f,   0.0213866625f, 0.009749332443f,
  3.381513724e-18f,   -0.007519159f, -0.01266706176f, -0.01549717132f, -0.01623356342f,
    -0.0152332196f, -0.01293939352f,-0.009831513278f,-0.006377025973f,-0.002989985514f,
  -2.110346192e-18f, 0.002366269007f,  0.00399050815f, 0.004856021609f, 0.005031134468f,
   0.004645895679f, 0.003865677863f, 0.002865221584f,  0.00180617522f,0.0008203662583f,
  8.391786605e-19f,-0.0006050768425f,-0.0009838859551f,-0.001156177954f,-0.001161655295f,
  -0.001048665727f,-0.0008642434841f,-0.0006468905485f,-0.0004228431499f,-0.0002058673272f,
  -3.126438727e-19f
};

/* 80��FIR��ͨ�˲���ϵ�� ͨ��fadtool��ȡϵ�� Fs = 6Msps, Fc = 400KHz 60 */
const float32_t firCoeffs32LP_400KHz[NUM_TAPS] = {
  -0.0005509475013f,-0.0003903231118f,-0.0001490870927f,0.0001657114772f,0.0005324134254f,
  0.0009052493842f, 0.001210837392f, 0.001354795997f, 0.001239938312f,0.0007938564522f,
  -1.115305094e-18f,-0.001076314598f,-0.002276225016f,-0.003357433481f,-0.004029342905f,
  -0.004010519013f,-0.003099408234f,-0.001243948471f,  0.00140698324f, 0.004486430436f,
   0.007437791675f, 0.009591832757f,  0.01028504875f, 0.009001951665f,  0.00551596377f,
  -3.719059042e-18f,-0.006920243148f, -0.01418955531f, -0.02042979747f, -0.02412135899f,
   -0.02383589372f, -0.01848467067f,-0.007540562656f,  0.00880737137f,  0.02959906682f,
     0.0531655848f,  0.07730623335f,  0.09955876321f,   0.1175239831f,   0.1291952729f,
     0.1332411021f,   0.1291952729f,   0.1175239831f,  0.09955876321f,  0.07730623335f,
     0.0531655848f,  0.02959906682f,  0.00880737137f,-0.007540562656f, -0.01848467067f,
   -0.02383589372f, -0.02412135899f, -0.02042979747f, -0.01418955531f,-0.006920243148f,
  -3.719059042e-18f,  0.00551596377f, 0.009001951665f,  0.01028504875f, 0.009591832757f,
   0.007437791675f, 0.004486430436f,  0.00140698324f,-0.001243948471f,-0.003099408234f,
  -0.004010519013f,-0.004029342905f,-0.003357433481f,-0.002276225016f,-0.001076314598f,
  -1.115305094e-18f,0.0007938564522f, 0.001239938312f, 0.001354795997f, 0.001210837392f,
  0.0009052493842f,0.0005324134254f,0.0001657114772f,-0.0001490870927f,-0.0003903231118f,
  -0.0005509475013f
};

/* 80��FIR��ͨ�˲���ϵ�� ͨ��fadtool��ȡϵ�� Fs = 6Msps, Fc = 500KHz 60 */
const float32_t firCoeffs32LP_500KHz[NUM_TAPS] = {
  0.0005504951696f,0.0006635120953f,0.0006204903475f,0.0003981869086f,-6.650214694e-19f,
  -0.0005222168984f,-0.001053526299f,-0.001423347159f,-0.001443778398f,-0.000975083909f,
  1.392986834e-18f, 0.001322023687f, 0.002650425769f, 0.003527316032f, 0.003505853703f,
   0.002313573146f,-2.580918076e-18f,-0.002989074681f,-0.005855768919f,-0.007626504637f,
  -0.007431684993f,-0.004818374291f,3.969862271e-18f, 0.006051687989f,  0.01173497085f,
    0.01517174393f,  0.01472251397f, 0.009539126419f,-5.257047121e-18f, -0.01211715676f,
   -0.02381632291f, -0.03142217919f, -0.03138330951f, -0.02116316929f,6.161883036e-18f,
    0.03066996299f,  0.06726266444f,   0.1045963317f,   0.1368443817f,   0.1586889178f,
     0.1664146334f,   0.1586889178f,   0.1368443817f,   0.1045963317f,  0.06726266444f,
    0.03066996299f,6.161883036e-18f, -0.02116316929f, -0.03138330951f, -0.03142217919f,
   -0.02381632291f, -0.01211715676f,-5.257047121e-18f, 0.009539126419f,  0.01472251397f,
    0.01517174393f,  0.01173497085f, 0.006051687989f,3.969862271e-18f,-0.004818374291f,
  -0.007431684993f,-0.007626504637f,-0.005855768919f,-0.002989074681f,-2.580918076e-18f,
   0.002313573146f, 0.003505853703f, 0.003527316032f, 0.002650425769f, 0.001322023687f,
  1.392986834e-18f,-0.000975083909f,-0.001443778398f,-0.001423347159f,-0.001053526299f,
  -0.0005222168984f,-6.650214694e-19f,0.0003981869086f,0.0006204903475f,0.0006635120953f,
  0.0005504951696f
};

/* 80��FIR��ͨ�˲���ϵ�� ͨ��fadtool��ȡϵ�� Fs = 6Msps, Fc = 600KHz 60 */
const float32_t firCoeffs32LP_600KHz[NUM_TAPS] = {
  -6.248144429e-19f,-0.0003912865068f,-0.0006836566608f,-0.0007598898374f,-0.0005337275215f,
  8.982911585e-19f,0.0007173990016f, 0.001358139911f, 0.001590756001f, 0.001150053344f,
  -1.677086875e-18f,-0.001559248194f,-0.002920240862f,-0.003365720157f,-0.002387311775f,
  2.842637194e-18f,  0.00310705835f, 0.005704274401f, 0.006451890338f, 0.004497503862f,
  -4.217497399e-18f,-0.005682985298f, -0.01031043474f, -0.01154888794f,-0.007990931161f,
  5.592357604e-18f,  0.01002530009f,  0.01820422709f,    0.020480223f,  0.01429146435f,
  -6.757908027e-18f,  -0.0185302943f,   -0.034578152f, -0.04038725421f, -0.02967212349f,
   7.53670395e-18f,   0.0458025299f,   0.0998044908f,    0.150775224f,   0.1871641278f,
     0.2003549486f,   0.1871641278f,    0.150775224f,   0.0998044908f,   0.0458025299f,
   7.53670395e-18f, -0.02967212349f, -0.04038725421f,   -0.034578152f,  -0.0185302943f,
  -6.757908027e-18f,  0.01429146435,    0.020480223f,  0.01820422709f,  0.01002530009f,
  5.592357604e-18f,-0.007990931161f, -0.01154888794f, -0.01031043474f,-0.005682985298f,
  -4.217497399e-18f, 0.004497503862f, 0.006451890338f, 0.005704274401f,  0.00310705835f,
  2.842637194e-18f,-0.002387311775f,-0.003365720157f,-0.002920240862f,-0.001559248194f,
  -1.677086875e-18f, 0.001150053344f, 0.001590756001f, 0.001358139911f,0.0007173990016f,
  8.982911585e-19f,-0.0005337275215f,-0.0007598898374f,-0.0006836566608f,-0.0003912865068f,
  -6.248144429e-19f
};

/* 80��FIR��ͨ�˲���ϵ�� ͨ��fadtool��ȡϵ�� Fs = 6Msps, Fc = 700KHz */
const float32_t firCoeffs32LP_700KHz[NUM_TAPS] = {
  -0.0005511845229f,-0.0002052932687f,0.0002917837992f,0.0007284347084f,0.0008618335123f,
  0.0005228708033f,-0.0002532428189f,-0.001152953948f,-0.001660075039f,-0.001306550927f,
  1.952623529e-18f, 0.001771428389f, 0.003047493752f, 0.002857232001f,0.0008427243447f,
  -0.002316470258f,-0.005017105024f,-0.005468149669f,-0.002753658686f, 0.002359670587f,
   0.007440990768f, 0.009437965229f, 0.006359243765f,-0.001266731415f, -0.01008250285f,
   -0.01519074198f, -0.01264935266f,-0.001996717649f,  0.01263173949f,  0.02373441868f,
    0.02384614572f, 0.009722146206f, -0.01475791167f, -0.03871545196f, -0.04791289568f,
   -0.03070836887f,  0.01616835408f,  0.08472616971f,    0.157345444f,   0.2126331627f,
     0.2332722247f,   0.2126331627f,    0.157345444f,  0.08472616971f,  0.01616835408f,
   -0.03070836887f, -0.04791289568f, -0.03871545196f, -0.01475791167f, 0.009722146206f,
    0.02384614572f,  0.02373441868f,  0.01263173949f,-0.001996717649f, -0.01264935266f,
   -0.01519074198f, -0.01008250285f,-0.001266731415f, 0.006359243765f, 0.009437965229f,
   0.007440990768f, 0.002359670587f,-0.002753658686f,-0.005468149669f,-0.005017105024f,
  -0.002316470258f,0.0008427243447f, 0.002857232001f, 0.003047493752f, 0.001771428389f,
  1.952623529e-18f,-0.001306550927f,-0.001660075039f,-0.001152953948f,-0.0002532428189f,
  0.0005228708033f,0.0008618335123f,0.0007284347084f,0.0002917837992f,-0.0002052932687f,
  -0.0005511845229f
};

/* 80��FIR��ͨ�˲���ϵ�� ͨ��fadtool��ȡϵ�� Fs = 6Msps, Fc = 800KHz */
const float32_t firCoeffs32LP_800KHz[NUM_TAPS] = {
  0.0005506729358f,0.0006312413025f,0.0002915130171f,-0.0003240190272f,-0.0008610336808f,
  -0.0009047982749f,-0.0002530078054f,0.0008368926356f, 0.001658534398f, 0.001449725125f,
  -2.229498664e-18f,-0.001965544652f,-0.003044665558f,-0.002073973883f,0.0008419422084f,
   0.004008520395f, 0.005012448877f, 0.002432317706f,-0.002751102904f,-0.007255579345f,
  -0.007434085011f,-0.002004239708f, 0.006353341974f,  0.01204095967f,  0.01007314492f,
  -7.434411487e-18f, -0.01263761241f, -0.01897986792f, -0.01262001693f, 0.005040224176f,
    0.02382401563f,  0.02989391983f,  0.01474421471f, -0.01722123288f, -0.04786842689f,
   -0.05313909054f, -0.01615334861f,  0.06150003523f,   0.1571994126f,   0.2359338701f,
     0.2663494051f,   0.2359338701f,   0.1571994126f,  0.06150003523f, -0.01615334861f,
   -0.05313909054f, -0.04786842689f, -0.01722123288f,  0.01474421471f,  0.02989391983f,
    0.02382401563f, 0.005040224176f, -0.01262001693f, -0.01897986792f, -0.01263761241f,
  -7.434411487e-18f,  0.01007314492f,  0.01204095967f, 0.006353341974f,-0.002004239708f,
  -0.007434085011f,-0.007255579345f,-0.002751102904f, 0.002432317706f, 0.005012448877f,
   0.004008520395f,0.0008419422084f,-0.002073973883f,-0.003044665558f,-0.001965544652f,
  -2.229498664e-18f, 0.001449725125f, 0.001658534398f,0.0008368926356f,-0.0002530078054f,
  -0.0009047982749f,-0.0008610336808f,-0.0003240190272f,0.0002915130171f,0.0006312413025f,
  0.0005506729358f
};

/* 80��FIR��ͨ�˲���ϵ�� ͨ��fadtool��ȡϵ�� Fs = 6Msps, Fc = 900KHz */
const float32_t firCoeffs32LP_900KHz[NUM_TAPS] = {
  -9.366794733e-19f,-0.0005382481613f,-0.0006832612562f,-0.0002467603481f,0.0005334187881f,
   0.001047265832f,0.0007169840392f,-0.0004410311521f,-0.001589835854f,-0.001581997029f,
  2.514175147e-18f, 0.002144879662f, 0.002918551676f, 0.001092956285f, -0.00238593109f,
  -0.004639693536f, -0.00310526113f, 0.001852359041f, 0.006448158063f, 0.006186702754f,
  -6.322586859e-18f,-0.007817434147f, -0.01030447055f,-0.003750290489f, 0.007986309007f,
    0.01521288417f,  0.01001950074f, -0.00591149088f, -0.02046837471f, -0.01965913549f,
  1.013099795e-17f,  0.02549001016f,  0.03455815092f,  0.01311502419f, -0.02965495922f,
   -0.06150625646f, -0.04577603564f,   0.0324096866f,   0.1506880075f,    0.257460326f,
     0.3003585935f,    0.257460326f,   0.1506880075f,   0.0324096866f, -0.04577603564f,
   -0.06150625646f, -0.02965495922f,  0.01311502419f,  0.03455815092f,  0.02549001016f,
  1.013099795e-17f, -0.01965913549f, -0.02046837471f, -0.00591149088f,  0.01001950074f,
    0.01521288417f, 0.007986309007f,-0.003750290489f, -0.01030447055f,-0.007817434147f,
  -6.322586859e-18f, 0.006186702754f, 0.006448158063f, 0.001852359041f, -0.00310526113f,
  -0.004639693536f, -0.00238593109f, 0.001092956285f, 0.002918551676f, 0.002144879662f,
  2.514175147e-18f,-0.001581997029f,-0.001589835854f,-0.0004410311521f,0.0007169840392f,
   0.001047265832f,0.0005334187881f,-0.0002467603481f,-0.0006832612562f,-0.0005382481613f,
  -9.366794733e-19f
};

/* 80��FIR��ͨ�˲���ϵ�� ͨ��fadtool��ȡϵ�� Fs = 6Msps, Fc = 1000KHz */
const float32_t firCoeffs32LP_1000KHz[NUM_TAPS] = {
  -0.0005513262586f,-1.302894147e-18f,0.0006214270834f,  0.00069072115f,-1.33205092e-18f,
  -0.0009058716823f,-0.001055116765f,6.984680832e-18f, 0.001445958042f, 0.001691444544f,
  -2.790179468e-18f,-0.002293268917f,-0.002654426964f, 3.89361359e-18f, 0.003511146177f,
   0.004013275728f,-5.16962902e-18f,-0.005185044836f,-0.005864609499f,6.547712334e-18f,
   0.007442904171f, 0.008358268067f,-7.95171039e-18f, -0.01049765479f, -0.01175268739f,
  9.304038194e-18f,  0.01474473998f,  0.01654719561f,-1.052996701e-17f, -0.02101921476f,
   -0.02385227755f,1.156174992e-17f,  0.03143068776f,  0.03671102226f,-1.234237102e-17f,
   -0.05320213363f,  -0.0673642084f,1.282869366e-17f,   0.1370509714f,   0.2752722204f,
     0.3333317339f,   0.2752722204f,   0.1370509714f,1.282869366e-17f,  -0.0673642084f,
   -0.05320213363f,-1.234237102e-17f,  0.03671102226f,  0.03143068776f,1.156174992e-17f,
   -0.02385227755f, -0.02101921476f,-1.052996701e-17f,  0.01654719561f,  0.01474473998f,
  9.304038194e-18f, -0.01175268739f, -0.01049765479f,-7.95171039e-18f, 0.008358268067f,
   0.007442904171f,6.547712334e-18f,-0.005864609499f,-0.005185044836f,-5.16962902e-18f,
   0.004013275728f, 0.003511146177f, 3.89361359e-18,-0.002654426964f,-0.002293268917f,
  -2.790179468e-18f, 0.001691444544f, 0.001445958042f,6.984680832e-18f,-0.001055116765f,
  -0.0009058716823f,-1.33205092e-18f,  0.00069072115f,0.0006214270834f,-1.302894147e-18f,
  -0.0005513262586f
};

/*
*********************************************************************************************************
*	�� �� ��: DSO_FirFilter_Step100KHz
*	����˵��: FIR�˲�����
*	��    ��: ��        	
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DSO_FirFilter_Step100KHz(void)
{
	arm_fir_instance_f32 S;
	uint16_t i;
	
	/* ��ȡҪ�˲�����ֵ */
	for(i = 0; i < BLOCK_SIZE; i++)
	{
		FirDataInput[i] =g_usWaveBuf1[i+g_usCurTriPos+g_usCurTriStep];
	}
	
	/* FIR��ͨ�˲�������ֹƵ��100KHz */
	if(g_ucFirFlter_Step100KHz == 1)
	{
		arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32LP_100KHz[0], &firStateF32[0], BLOCK_SIZE);
	}
	/* FIR��ͨ�˲�������ֹƵ��200KHz */
	else if(g_ucFirFlter_Step100KHz == 2)
	{
		arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32LP_200KHz[0], &firStateF32[0], BLOCK_SIZE);
	}
	/* FIR��ͨ�˲�������ֹƵ��300KHz */
	else if(g_ucFirFlter_Step100KHz == 3)
	{
		arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32LP_300KHz[0], &firStateF32[0], BLOCK_SIZE);
	}
	/* FIR��ͨ�˲�������ֹƵ��400KHz */
	else if(g_ucFirFlter_Step100KHz == 4)
	{
		arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32LP_400KHz[0], &firStateF32[0], BLOCK_SIZE);
	}
	/* FIR��ͨ�˲�������ֹƵ��500KHz */
	else if(g_ucFirFlter_Step100KHz == 5)
	{
		arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32LP_500KHz[0], &firStateF32[0], BLOCK_SIZE);
	}
	/* FIR��ͨ�˲�������ֹƵ��600KHz */
	else if(g_ucFirFlter_Step100KHz == 6)
	{
		arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32LP_600KHz[0], &firStateF32[0], BLOCK_SIZE);
	}
	/* FIR��ͨ�˲�������ֹƵ��700KHz */
	else if(g_ucFirFlter_Step100KHz == 7)
	{
		arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32LP_700KHz[0], &firStateF32[0], BLOCK_SIZE);
	}
	/* FIR��ͨ�˲�������ֹƵ��800KHz */
	else if(g_ucFirFlter_Step100KHz == 8)
	{
		arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32LP_800KHz[0], &firStateF32[0], BLOCK_SIZE);
	}
	/* FIR��ͨ�˲�������ֹƵ��900KHz */
	else if(g_ucFirFlter_Step100KHz == 9)
	{
		arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32LP_900KHz[0], &firStateF32[0], BLOCK_SIZE);
	}
	/* FIR��ͨ�˲�������ֹƵ��1000KHz */
	else if(g_ucFirFlter_Step100KHz == 10)
	{
		arm_fir_init_f32(&S, NUM_TAPS, (float32_t *)&firCoeffs32LP_1000KHz[0], &firStateF32[0], BLOCK_SIZE);
	}
	
	/* 80��FIR�˲� */
	arm_fir_f32(&S, FirDataInput, FirDataOutput, BLOCK_SIZE);
	
	/* ����˲�����ֵ */
	for(i = 0; i < DSOSCREEN_LENGTH; i++)
	{
		g_usWaveBuf[i] =FirDataOutput[i+50];
	}
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/