declare namespace DsDialogSystem {
    interface TextBox {
        type: 'TextBox';
        label: string;
        defaultValue?: string;
    }
    
    interface TextArea {
        type: 'TextArea';
        label: string;
        defaultValue?: string
    }

    interface NumberBox {
        type: 'NumberBox';
        label: string;
        precision?: number;
        min?: number;
        max?: number;
        step?: number;
        prefix?: string;
        suffix?: string;
        defaultValue?: number;
    }
    
    interface KeyNameBox {
        type: 'KeyNameBox';
        label: string;
        min?: number;
        max?: number;
        defaultValue?: number;
    }
    
    interface TimeBox {
        type: 'TimeBox';
        label: string;
        format: 'MBT'|'HMS';
        relative?: boolean;
        defaultValue?: number;
    }
    
    interface CheckBox {
        type: 'CheckBox';
        label: string;
        defaultValue?: boolean;
    }
    
    interface Select {
        type: 'Select';
        label: string;
        options: string[];
        defaultValue?: number;
    }
    
    interface Label {
        type: 'Label';
        label: string;
    }

    type FormWidget = TextBox | TextArea | NumberBox | Slider | KeyNameBox | TimeBox | CheckBox | Select | Label;

    type FormReturnType<T> = T extends TextBox ? string:
                             T extends TextArea ? string:
                             T extends NumberBox ? number:
                             T extends Slider ? number:
                             T extends KeyNameBox ? number:
                             T extends TimeBox ? number:
                             T extends CheckBox ? boolean:
                             T extends Select ? number:
                             T extends Label ? undefined:
                             never;

    interface DialogReturnValue<T extends FormWidget[]> {
        form: {
            [K in keyof T]: FormReturnType<T[K]>
        }
        result: 'Ok'|'Cancel';
    }

    /**
     * The parameter type of listener callback in {@link DS.dialogSystem.form}.
     */
    type DialogWidgetHandles<T extends FormWidget[]> = {
        [K in keyof T]: {

            /**
             * The value of current widget. This property is readonly.
             */
            value: readonly FormReturnType<T[K]>;

            /**
             * Whether current widget is visible or not. This property can be both get and set.
             */
            visible: boolean;

            /**
             * Whether current widget is disabled or not. This property can be both get and set.
             */
            disabled: boolean;
        }
    }
}

declare global {

    /**
     * The extension of Qt {@link https://doc.qt.io/qt-5/qcoreapplication.html#translate | QCoreApplication::translate } function.
     * @param context 
     * @param sourceText 
     * @param disambiguation 
     * @param n 
     */
    function $tr(context: string, sourceText: string, disambiguation?: string, n?: number): string;

    /**
     * Register a batch process script
     * @param clazz The class of script
     */
    function $register(clazz: new() => Script|ScriptSet): void;

    interface ScriptLocale {
        [langCode: string]: {
            [sourceText: string]: string
        }
    }

    interface ScriptInfoBase {
        id: string;
        name: string;
        role: string|string[];
        requiredEditorVersion: string;
    }

    abstract class ScriptBase {
        public constructor(ds: DS);

        /**
         * Override this method to provide the translations.
         * @returns Translations
         */
        protected abstract static locale(): ScriptLocale;

        /**
         * An utility for i18n. The source text will be indexed in the return value of {@link locale}.
         * @param s Source text
         * @returns translation
         */
        protected static tr(s: string): string;

        /**
         * Override this method to provide the information of script.
         */
        public abstract static info(): ScriptInfoBase;

        /**
         * APIs provided by DiffScope Editor.
         */
        protected ds:DS;
    }

    interface ScriptInfo extends ScriptInfoBase {
        shortcut: string;
    }

    /**
     * A single batch process action.
     */
    abstract class $Script extends ScriptBase {
        /**
         * The main entry of script.
         */
        public abstract main(): void;
        public override abstract static info(): ScriptInfo;
    }

    interface ScriptSetInfo extends ScriptInfoBase {
        children: {
            id: string;
            name: string;
            shortcut: string;
        }[];
    }

    /**
     * A set of batch process actions.
     */
    abstract class $ScriptSet extends ScriptBase {
        /**
         * The main entry of script.
         * @param index Index of child script in script set
         */
        public abstract main(index: number): void;
        public override abstract static info(): ScriptSetInfo;
    }

    /**
     * Type definitions of APIs provided by DiffScope Editor.
     */
    interface DS {

        /**
         * Methods to interact with DiffScope dialog system
         */
        dialogSystem: {

            /**
             * Show a popup modal dialog.
             * @param title The title of dialog
             * @param widgets The widgets dialog contains
             * @param listener The listener that listens for value changed events of dialog widgets
             * @returns Dialog result
             */
            form<T extends DsDialogSystem.FormWidget[]>(
                title: string,
                widgets: T,
                listener: (handles: DsDialogSystem.DialogWidgetHandles) => void
            ): DsDialogSystem.FormReturnType<T>;

            /**
             * Show a information dialog.
             * @param title 
             * @param message 
             */
            alert(title: string, message: string): void;

            /**
             * Show a question dialog with yes-button and no-button.
             * @param title 
             * @param message 
             * @param defaultButton The button focused by default, yes-button if not specified
             * @returns Whether yes-button is selected
             */
            confirm(title: string, message: string, defaultButton?: string): boolean;

            /**
             * Show a dialog with a text box
             * @param title 
             * @param message 
             * @param defaultValue 
             * @returns The value user input, `null` if user cancelled
             */
            prompt(title: string, message: string, defaultValue?: string): string|null;
        }
    }
}