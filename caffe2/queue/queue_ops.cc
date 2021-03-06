#include "queue_ops.h"
#include <memory>

namespace caffe2 {

CAFFE_KNOWN_TYPE(std::shared_ptr<BlobsQueue>);

namespace {

REGISTER_CPU_OPERATOR(CreateBlobsQueue, CreateBlobsQueueOp<CPUContext>);
REGISTER_CPU_OPERATOR(EnqueueBlobs, EnqueueBlobsOp<CPUContext>);
REGISTER_CPU_OPERATOR(DequeueBlobs, DequeueBlobsOp<CPUContext>);
REGISTER_CPU_OPERATOR(CloseBlobsQueue, CloseBlobsQueueOp<CPUContext>);

REGISTER_CPU_OPERATOR(SafeEnqueueBlobs, SafeEnqueueBlobsOp<CPUContext>);
REGISTER_CPU_OPERATOR(SafeDequeueBlobs, SafeDequeueBlobsOp<CPUContext>);

OPERATOR_SCHEMA(CreateBlobsQueue).NumInputs(0).NumOutputs(1);
OPERATOR_SCHEMA(EnqueueBlobs)
    .NumInputsOutputs([](int inputs, int outputs) {
      return inputs >= 2 && outputs >= 1 && inputs == outputs + 1;
    })
    .EnforceInplace([](int input, int output) { return input == output + 1; });
OPERATOR_SCHEMA(DequeueBlobs).NumInputsOutputs([](int inputs, int outputs) {
  return inputs == 1 && outputs >= 1;
});
OPERATOR_SCHEMA(CloseBlobsQueue).NumInputs(1).NumOutputs(0);

OPERATOR_SCHEMA(SafeEnqueueBlobs)
    .NumInputsOutputs([](int inputs, int outputs) {
      return inputs >= 2 && outputs >= 2 && inputs == outputs;
    })
    .EnforceInplace([](int input, int output) { return input == output + 1; })
    .SetDoc(R"DOC(
Enqueue the blobs into queue. When the queue is closed and full, the output
status will be set to true which can be used as exit criteria for execution
step.
The 1st input is the queue and the last output is the status. The rest are
data blobs.
)DOC")
    .Input(0, "queue", "The shared pointer for the BlobsQueue");

OPERATOR_SCHEMA(SafeDequeueBlobs)
    .NumInputsOutputs([](int inputs, int outputs) {
      return inputs == 1 && outputs >= 2;
    })
    .SetDoc(R"DOC(
Dequeue the blobs from queue. When the queue is closed and empty, the output
status will be set to true which can be used as exit criteria for execution
step.
The 1st input is the queue and the last output is the status. The rest are
data blobs.
)DOC")
    .Input(0, "queue", "The shared pointer for the BlobsQueue");

NO_GRADIENT(CreateBlobsQueue);
NO_GRADIENT(EnqueueBlobs);
NO_GRADIENT(DequeueBlobs);
NO_GRADIENT(CloseBlobsQueue);

NO_GRADIENT(SafeEnqueueBlobsQueue);
NO_GRADIENT(SafeDequeueBlobsQueue);
}

}
