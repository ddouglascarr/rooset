import * as $protobuf from "protobufjs";
/** Namespace messages. */
export namespace messages {

    /** Properties of a Blob. */
    interface IBlob {

        /** Blob SHA */
        SHA?: (string|null);

        /** Blob Content */
        Content?: (string|null);
    }

    /** Represents a Blob. */
    class Blob implements IBlob {

        /**
         * Constructs a new Blob.
         * @param [properties] Properties to set
         */
        constructor(properties?: messages.IBlob);

        /** Blob SHA. */
        public SHA: string;

        /** Blob Content. */
        public Content: string;

        /**
         * Verifies a Blob message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a Blob message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns Blob
         */
        public static fromObject(object: { [k: string]: any }): messages.Blob;

        /**
         * Creates a plain object from a Blob message. Also converts values to other types if specified.
         * @param message Blob
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: messages.Blob, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this Blob to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** FileActionType enum. */
    enum FileActionType {
        Create = 0,
        Update = 1
    }

    /** Properties of a FileAction. */
    interface IFileAction {

        /** FileAction Action */
        Action?: (messages.FileActionType|null);

        /** FileAction FileName */
        FileName?: (string|null);

        /** FileAction Content */
        Content?: (string|null);
    }

    /** Represents a FileAction. */
    class FileAction implements IFileAction {

        /**
         * Constructs a new FileAction.
         * @param [properties] Properties to set
         */
        constructor(properties?: messages.IFileAction);

        /** FileAction Action. */
        public Action: messages.FileActionType;

        /** FileAction FileName. */
        public FileName: string;

        /** FileAction Content. */
        public Content: string;

        /**
         * Verifies a FileAction message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a FileAction message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns FileAction
         */
        public static fromObject(object: { [k: string]: any }): messages.FileAction;

        /**
         * Creates a plain object from a FileAction message. Also converts values to other types if specified.
         * @param message FileAction
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: messages.FileAction, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this FileAction to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a CommitRecord. */
    interface ICommitRecord {

        /** CommitRecord SHA */
        SHA?: (string|null);

        /** CommitRecord BranchName */
        BranchName?: (string|null);
    }

    /** Represents a CommitRecord. */
    class CommitRecord implements ICommitRecord {

        /**
         * Constructs a new CommitRecord.
         * @param [properties] Properties to set
         */
        constructor(properties?: messages.ICommitRecord);

        /** CommitRecord SHA. */
        public SHA: string;

        /** CommitRecord BranchName. */
        public BranchName: string;

        /**
         * Verifies a CommitRecord message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a CommitRecord message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns CommitRecord
         */
        public static fromObject(object: { [k: string]: any }): messages.CommitRecord;

        /**
         * Creates a plain object from a CommitRecord message. Also converts values to other types if specified.
         * @param message CommitRecord
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: messages.CommitRecord, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this CommitRecord to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a BlobRecord. */
    interface IBlobRecord {

        /** BlobRecord SHA */
        SHA?: (string|null);

        /** BlobRecord Name */
        Name?: (string|null);
    }

    /** Represents a BlobRecord. */
    class BlobRecord implements IBlobRecord {

        /**
         * Constructs a new BlobRecord.
         * @param [properties] Properties to set
         */
        constructor(properties?: messages.IBlobRecord);

        /** BlobRecord SHA. */
        public SHA: string;

        /** BlobRecord Name. */
        public Name: string;

        /**
         * Verifies a BlobRecord message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a BlobRecord message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns BlobRecord
         */
        public static fromObject(object: { [k: string]: any }): messages.BlobRecord;

        /**
         * Creates a plain object from a BlobRecord message. Also converts values to other types if specified.
         * @param message BlobRecord
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: messages.BlobRecord, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this BlobRecord to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a NewInitiativeReq. */
    interface INewInitiativeReq {

        /** NewInitiativeReq Content */
        Content?: (string|null);
    }

    /** Represents a NewInitiativeReq. */
    class NewInitiativeReq implements INewInitiativeReq {

        /**
         * Constructs a new NewInitiativeReq.
         * @param [properties] Properties to set
         */
        constructor(properties?: messages.INewInitiativeReq);

        /** NewInitiativeReq Content. */
        public Content: string;

        /**
         * Verifies a NewInitiativeReq message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a NewInitiativeReq message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns NewInitiativeReq
         */
        public static fromObject(object: { [k: string]: any }): messages.NewInitiativeReq;

        /**
         * Creates a plain object from a NewInitiativeReq message. Also converts values to other types if specified.
         * @param message NewInitiativeReq
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: messages.NewInitiativeReq, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this NewInitiativeReq to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a NewInitiativeResp. */
    interface INewInitiativeResp {

        /** NewInitiativeResp CommitRecord */
        CommitRecord?: (messages.ICommitRecord|null);
    }

    /** Represents a NewInitiativeResp. */
    class NewInitiativeResp implements INewInitiativeResp {

        /**
         * Constructs a new NewInitiativeResp.
         * @param [properties] Properties to set
         */
        constructor(properties?: messages.INewInitiativeResp);

        /** NewInitiativeResp CommitRecord. */
        public CommitRecord?: (messages.ICommitRecord|null);

        /**
         * Verifies a NewInitiativeResp message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a NewInitiativeResp message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns NewInitiativeResp
         */
        public static fromObject(object: { [k: string]: any }): messages.NewInitiativeResp;

        /**
         * Creates a plain object from a NewInitiativeResp message. Also converts values to other types if specified.
         * @param message NewInitiativeResp
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: messages.NewInitiativeResp, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this NewInitiativeResp to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a GetDocReq. */
    interface IGetDocReq {

        /** GetDocReq GitRef */
        GitRef?: (string|null);
    }

    /** Represents a GetDocReq. */
    class GetDocReq implements IGetDocReq {

        /**
         * Constructs a new GetDocReq.
         * @param [properties] Properties to set
         */
        constructor(properties?: messages.IGetDocReq);

        /** GetDocReq GitRef. */
        public GitRef: string;

        /**
         * Verifies a GetDocReq message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a GetDocReq message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns GetDocReq
         */
        public static fromObject(object: { [k: string]: any }): messages.GetDocReq;

        /**
         * Creates a plain object from a GetDocReq message. Also converts values to other types if specified.
         * @param message GetDocReq
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: messages.GetDocReq, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this GetDocReq to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a GetDocResp. */
    interface IGetDocResp {

        /** GetDocResp Blob */
        Blob?: (messages.IBlob|null);
    }

    /** Represents a GetDocResp. */
    class GetDocResp implements IGetDocResp {

        /**
         * Constructs a new GetDocResp.
         * @param [properties] Properties to set
         */
        constructor(properties?: messages.IGetDocResp);

        /** GetDocResp Blob. */
        public Blob?: (messages.IBlob|null);

        /**
         * Verifies a GetDocResp message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a GetDocResp message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns GetDocResp
         */
        public static fromObject(object: { [k: string]: any }): messages.GetDocResp;

        /**
         * Creates a plain object from a GetDocResp message. Also converts values to other types if specified.
         * @param message GetDocResp
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: messages.GetDocResp, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this GetDocResp to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a GetBlobReq. */
    interface IGetBlobReq {

        /** GetBlobReq SHA */
        SHA?: (string|null);
    }

    /** Represents a GetBlobReq. */
    class GetBlobReq implements IGetBlobReq {

        /**
         * Constructs a new GetBlobReq.
         * @param [properties] Properties to set
         */
        constructor(properties?: messages.IGetBlobReq);

        /** GetBlobReq SHA. */
        public SHA: string;

        /**
         * Verifies a GetBlobReq message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a GetBlobReq message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns GetBlobReq
         */
        public static fromObject(object: { [k: string]: any }): messages.GetBlobReq;

        /**
         * Creates a plain object from a GetBlobReq message. Also converts values to other types if specified.
         * @param message GetBlobReq
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: messages.GetBlobReq, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this GetBlobReq to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a GetBlobResp. */
    interface IGetBlobResp {

        /** GetBlobResp Blob */
        Blob?: (messages.IBlob|null);
    }

    /** Represents a GetBlobResp. */
    class GetBlobResp implements IGetBlobResp {

        /**
         * Constructs a new GetBlobResp.
         * @param [properties] Properties to set
         */
        constructor(properties?: messages.IGetBlobResp);

        /** GetBlobResp Blob. */
        public Blob?: (messages.IBlob|null);

        /**
         * Verifies a GetBlobResp message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a GetBlobResp message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns GetBlobResp
         */
        public static fromObject(object: { [k: string]: any }): messages.GetBlobResp;

        /**
         * Creates a plain object from a GetBlobResp message. Also converts values to other types if specified.
         * @param message GetBlobResp
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: messages.GetBlobResp, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this GetBlobResp to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a ListAreaBlobsReq. */
    interface IListAreaBlobsReq {

        /** ListAreaBlobsReq SHA */
        SHA?: (string|null);
    }

    /** Represents a ListAreaBlobsReq. */
    class ListAreaBlobsReq implements IListAreaBlobsReq {

        /**
         * Constructs a new ListAreaBlobsReq.
         * @param [properties] Properties to set
         */
        constructor(properties?: messages.IListAreaBlobsReq);

        /** ListAreaBlobsReq SHA. */
        public SHA: string;

        /**
         * Verifies a ListAreaBlobsReq message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a ListAreaBlobsReq message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns ListAreaBlobsReq
         */
        public static fromObject(object: { [k: string]: any }): messages.ListAreaBlobsReq;

        /**
         * Creates a plain object from a ListAreaBlobsReq message. Also converts values to other types if specified.
         * @param message ListAreaBlobsReq
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: messages.ListAreaBlobsReq, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this ListAreaBlobsReq to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }

    /** Properties of a ListAreaBlobsResp. */
    interface IListAreaBlobsResp {

        /** ListAreaBlobsResp BlobRecords */
        BlobRecords?: (messages.IBlobRecord[]|null);
    }

    /** Represents a ListAreaBlobsResp. */
    class ListAreaBlobsResp implements IListAreaBlobsResp {

        /**
         * Constructs a new ListAreaBlobsResp.
         * @param [properties] Properties to set
         */
        constructor(properties?: messages.IListAreaBlobsResp);

        /** ListAreaBlobsResp BlobRecords. */
        public BlobRecords: messages.IBlobRecord[];

        /**
         * Verifies a ListAreaBlobsResp message.
         * @param message Plain object to verify
         * @returns `null` if valid, otherwise the reason why it is not
         */
        public static verify(message: { [k: string]: any }): (string|null);

        /**
         * Creates a ListAreaBlobsResp message from a plain object. Also converts values to their respective internal types.
         * @param object Plain object
         * @returns ListAreaBlobsResp
         */
        public static fromObject(object: { [k: string]: any }): messages.ListAreaBlobsResp;

        /**
         * Creates a plain object from a ListAreaBlobsResp message. Also converts values to other types if specified.
         * @param message ListAreaBlobsResp
         * @param [options] Conversion options
         * @returns Plain object
         */
        public static toObject(message: messages.ListAreaBlobsResp, options?: $protobuf.IConversionOptions): { [k: string]: any };

        /**
         * Converts this ListAreaBlobsResp to JSON.
         * @returns JSON object
         */
        public toJSON(): { [k: string]: any };
    }
}
