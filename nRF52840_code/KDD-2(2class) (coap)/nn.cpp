extern "C" {
#include "contiki.h"
#include "tfliteModel.c"
#include "inputHandler.c"
#include "outputHandler.c"
}

#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

constexpr int kTensorArenaSize = 2000;
uint8_t tensor_arena[kTensorArenaSize];
}

// This constant determines the number of inferences to perform across the range
// of x values defined above. Since each inference takes time, the higher this
// number, the more time it will take to run through the entire range. The value
// of this constant can be tuned so that one full cycle takes a desired amount
// of time. Since different devices take different amounts of time to perform
// inference, this value should be defined per-device.
const int kInferencesPerCycle = 20;

// The name of this function is important for Arduino compatibility.
int setup() {
    printf("\nSetup TF Lite iniziato.\n");
  tflite::InitializeTarget();

  // Set up logging. Google style is to avoid globals or statics because of
  // lifetime uncertainty, but since this has a trivial destructor it's okay.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(model_tflite);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    TF_LITE_REPORT_ERROR(error_reporter,
                         "Model provided is schema version %d not equal "
                         "to supported version %d.",
                         model->version(), TFLITE_SCHEMA_VERSION);
    return -1;
  }

  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::AllOpsResolver resolver;

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();
  if (allocate_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
    return -1;
  }

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);

  inference_count = 0;

  return 0;
}

// The name of this function is important for Arduino compatibility.
int loop() {
  // prendo il primo array di input
  float* inArray = getInput();
    //carico l'array coi dati nell'array di input del modello
  for(int i=0; i<122; i++){
	input->data.f[i] = inArray[i];
  }
  // Invoke() fa eseguire l'inferenza all'interprete creato usando modello
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed.\n");
    printf("Errore nel calcolo inferenza.");
    return -1;
  }
  // Prelevo l'output
  float outputArray[2];
  for(int i=0; i<2; i++){
      outputArray[i] = output->data.f[i];
  }
  int atk = getOutput(outputArray);//print dell'output
  TF_LITE_REPORT_ERROR(error_reporter,
                        "\n[1=Normal; 0=Atk] \nModel Output: %d\n", atk);

  // Increment the inference_counter, and reset it if we have reached
  // the total number per cycleSS
  inference_count += 1;
  if (inference_count >= kInferencesPerCycle) inference_count = 0;

  return atk;
}

extern "C" int nn_setup(void)
{
  return setup();
}

extern "C" int nn_run_inference(void)
{
  return loop();
}
